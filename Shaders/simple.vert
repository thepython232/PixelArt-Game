#version 450

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inUv;

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec2 outUv;

layout(set = 0, binding = 0) uniform CameraUBO {
	mat4 model;
	mat4 view;
	mat4 proj;
} camera;

void main() {
	gl_Position = camera.proj * camera.view * camera.model * vec4(inPos, 1.0);
	outColor = inColor;
	outUv = inUv;
}