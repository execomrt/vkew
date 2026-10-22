/*
 * -----------------------------------------------------------------------------
 *  File:        vkDeviceFault.h
 *  Description: VK_KHR_device_fault helpers: query the fault reports and the vendor
 *               post-mortem blob, and format them as text. Header only, depends on
 *               vkew.h alone. Enable the extension with VKEWSettings::enableDeviceFault.
 *
 *  Author:      Stéphane Denis
 *  Copyright:   (c) 2026 Stéphane Denis - www.v3x.net
 *  License:     MIT License
 * -----------------------------------------------------------------------------
 */
#pragma once
#include "vkew.h"

#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

#ifdef VK_KHR_device_fault
namespace vkewtest
{

/// One fault as reported by vkGetDeviceFaultReportsKHR.
struct DeviceFaultEntry
{
    VkDeviceFaultFlagsKHR flags = 0;
    uint64_t groupId = 0;
    std::string description;
    VkDeviceFaultAddressInfoKHR faultAddress{};
    VkDeviceFaultAddressInfoKHR instructionAddress{};
    std::string vendorDescription;
    uint64_t vendorFaultCode = 0;
    uint64_t vendorFaultData = 0;
};

/// Everything the extension can tell about a fault. `available` is false when the extension is not enabled.
struct DeviceFaultReport
{
    bool available = false;
    bool deviceLost = false;
    uint32_t maxFaultCount = 0;
    bool vendorBinaryFeature = false;
    VkResult countResult = VK_SUCCESS;
    VkResult reportsResult = VK_SUCCESS;
    VkResult debugInfoResult = VK_SUCCESS;
    std::vector<DeviceFaultEntry> faults;
    /// Vendor post-mortem blob. On NVIDIA this is a 56 byte VkDeviceFaultVendorBinaryHeaderVersionOneKHR followed by
    /// a crash dump. Only filled after device loss and when deviceFaultVendorBinary was enabled.
    std::vector<uint8_t> vendorBinary;
};

inline const char *DeviceFaultAddressTypeName(VkDeviceFaultAddressTypeKHR type)
{
    switch (type)
    {
    case VK_DEVICE_FAULT_ADDRESS_TYPE_NONE_KHR: return "none";
    case VK_DEVICE_FAULT_ADDRESS_TYPE_READ_INVALID_KHR: return "read invalid";
    case VK_DEVICE_FAULT_ADDRESS_TYPE_WRITE_INVALID_KHR: return "write invalid";
    case VK_DEVICE_FAULT_ADDRESS_TYPE_EXECUTE_INVALID_KHR: return "execute invalid";
    case VK_DEVICE_FAULT_ADDRESS_TYPE_INSTRUCTION_POINTER_UNKNOWN_KHR: return "instruction pointer unknown";
    case VK_DEVICE_FAULT_ADDRESS_TYPE_INSTRUCTION_POINTER_INVALID_KHR: return "instruction pointer invalid";
    case VK_DEVICE_FAULT_ADDRESS_TYPE_INSTRUCTION_POINTER_FAULT_KHR: return "instruction pointer fault";
    default: return "?";
    }
}

inline std::string DeviceFaultFlagsName(VkDeviceFaultFlagsKHR flags)
{
    std::string s;
    auto add = [&](VkDeviceFaultFlagBitsKHR bit, const char *name) {
        if (flags & bit)
            s += (s.empty() ? "" : "|") + std::string(name);
    };
    add(VK_DEVICE_FAULT_FLAG_DEVICE_LOST_KHR, "DEVICE_LOST");
    add(VK_DEVICE_FAULT_FLAG_MEMORY_ADDRESS_KHR, "MEMORY_ADDRESS");
    add(VK_DEVICE_FAULT_FLAG_INSTRUCTION_ADDRESS_KHR, "INSTRUCTION_ADDRESS");
    add(VK_DEVICE_FAULT_FLAG_VENDOR_KHR, "VENDOR");
    add(VK_DEVICE_FAULT_FLAG_WATCHDOG_TIMEOUT_KHR, "WATCHDOG_TIMEOUT");
    add(VK_DEVICE_FAULT_FLAG_OVERFLOW_KHR, "OVERFLOW");
    return s.empty() ? "0" : s;
}

/// Queries the fault reports (valid at any time on KHR, including faults the driver recovered from) and, when
/// `deviceLost` is true and the vendor binary feature is on, the vendor post-mortem blob.
inline DeviceFaultReport QueryDeviceFault(VkDevice device, bool deviceLost)
{
    DeviceFaultReport report;
    report.deviceLost = deviceLost;
    if (!VKEW_KHR_device_fault || !vkGetDeviceFaultReportsKHR)
        return report;
    report.available = true;
    report.vendorBinaryFeature = VKEW_KHR_device_fault_vendor_binary != 0;

    VkPhysicalDeviceFaultPropertiesKHR faultProps{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FAULT_PROPERTIES_KHR };
    VkPhysicalDeviceProperties2 props2{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2, &faultProps };
    vkGetPhysicalDeviceProperties2(vkewGetPhysicalDevice(), &props2);
    report.maxFaultCount = faultProps.maxDeviceFaultCount;

    // Two-call idiom: count first, then the reports.
    uint32_t count = 0;
    report.countResult = vkGetDeviceFaultReportsKHR(device, 0, &count, nullptr);
    if (report.countResult >= 0 && count > 0)
    {
        std::vector<VkDeviceFaultInfoKHR> infos(count);
        for (auto &info : infos)
        {
            info = {};
            info.sType = VK_STRUCTURE_TYPE_DEVICE_FAULT_INFO_KHR;
        }
        report.reportsResult = vkGetDeviceFaultReportsKHR(device, 0, &count, infos.data());
        if (report.reportsResult >= 0)
        {
            for (uint32_t i = 0; i < count && i < infos.size(); ++i)
            {
                const VkDeviceFaultInfoKHR &f = infos[i];
                DeviceFaultEntry entry;
                entry.flags = f.flags;
                entry.groupId = f.groupId;
                entry.description = f.description;
                entry.faultAddress = f.faultAddressInfo;
                entry.instructionAddress = f.instructionAddressInfo;
                entry.vendorDescription = f.vendorInfo.description;
                entry.vendorFaultCode = f.vendorInfo.vendorFaultCode;
                entry.vendorFaultData = f.vendorInfo.vendorFaultData;
                report.faults.push_back(std::move(entry));
            }
        }
    }

    // The vendor blob is only legal once the device is lost.
    if (deviceLost && report.vendorBinaryFeature && vkGetDeviceFaultDebugInfoKHR)
    {
        VkDeviceFaultDebugInfoKHR debug{ VK_STRUCTURE_TYPE_DEVICE_FAULT_DEBUG_INFO_KHR };
        report.debugInfoResult = vkGetDeviceFaultDebugInfoKHR(device, &debug);
        if (report.debugInfoResult >= 0 && debug.vendorBinarySize > 0)
        {
            report.vendorBinary.resize(debug.vendorBinarySize);
            debug.pVendorBinaryData = report.vendorBinary.data();
            report.debugInfoResult = vkGetDeviceFaultDebugInfoKHR(device, &debug);
            if (report.debugInfoResult < 0)
                report.vendorBinary.clear();
        }
    }
    return report;
}

/// Human readable multi-line text, one line per fact.
inline std::string FormatDeviceFaultReport(const DeviceFaultReport &report)
{
    std::string out;
    auto line = [&](const char *fmt, auto... args) {
        char buffer[1024];
        std::snprintf(buffer, sizeof(buffer), fmt, args...);
        out += buffer;
        out += '\n';
    };
    if (!report.available)
    {
        line("VK_KHR_device_fault is not enabled on this device; no fault report available.");
        return out;
    }
    line("maxDeviceFaultCount = %u, vendor binary = %s", report.maxFaultCount, report.vendorBinaryFeature ? "yes" : "no");
    line("vkGetDeviceFaultReportsKHR: count = %s, data = %s, faults = %zu", vkewVkResultToString(report.countResult),
         vkewVkResultToString(report.reportsResult), report.faults.size());
    for (size_t i = 0; i < report.faults.size(); ++i)
    {
        const DeviceFaultEntry &f = report.faults[i];
        line("--- fault %zu: flags %s, group %" PRIu64, i, DeviceFaultFlagsName(f.flags).c_str(), f.groupId);
        line("    description: %s", f.description.c_str());
        line("    fault address:       0x%016" PRIX64 " (%s, precision %" PRIu64 ")", static_cast<uint64_t>(f.faultAddress.reportedAddress),
             DeviceFaultAddressTypeName(f.faultAddress.addressType), static_cast<uint64_t>(f.faultAddress.addressPrecision));
        line("    instruction address: 0x%016" PRIX64 " (%s, precision %" PRIu64 ")", static_cast<uint64_t>(f.instructionAddress.reportedAddress),
             DeviceFaultAddressTypeName(f.instructionAddress.addressType), static_cast<uint64_t>(f.instructionAddress.addressPrecision));
        line("    vendor: '%s' code 0x%" PRIX64 " data 0x%" PRIX64, f.vendorDescription.c_str(), f.vendorFaultCode, f.vendorFaultData);
    }
    if (report.deviceLost && report.vendorBinaryFeature)
        line("vkGetDeviceFaultDebugInfoKHR = %s, vendor binary = %zu bytes", vkewVkResultToString(report.debugInfoResult), report.vendorBinary.size());
    return out;
}

} // namespace vkewtest
#endif // VK_KHR_device_fault
