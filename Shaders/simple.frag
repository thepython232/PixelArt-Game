#version 450

layout(location = 0) in vec3 inColor;
layout(location = 1) in vec2 inUv;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform sampler2D statue;

void main() {
	outColor = vec4(texture(statue, inUv).rgb * inColor, 1.0);
}