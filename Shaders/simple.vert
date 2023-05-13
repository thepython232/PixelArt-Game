#version 450

vec2 positions[] = {
	{ -0.5, 0.5 },
	{ 0.0, -0.5 },
	{ 0.5, 0.5 }
};

vec3 colors[] = {
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 }
};

layout(location = 0) out vec3 outColor;

void main() {
	gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
	outColor = colors[gl_VertexIndex];
}