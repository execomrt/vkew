@echo off
mkdir build
cd build
cmake -G "Visual Studio 17 2022" .. -DVKEW_ENABLE_TESTS="ON"
cd ..
echo Visual Studio project generated in build/
pause
