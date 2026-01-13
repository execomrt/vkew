#version 410 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 outColor;

out gl_PerVertex {
    vec4 gl_Position;
};

void main()
{
    gl_Position = vec4(inPosition, 1.0);
    outColor = inColor;
}