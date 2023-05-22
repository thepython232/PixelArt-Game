#include "Vertex.h"

namespace gfx {

	const VkVertexInputBindingDescription VertexPosColor::Binding = {
		.binding = 0,
		.stride = sizeof(VertexPosColor),
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
	};

	const std::vector<VkVertexInputAttributeDescription> VertexPosColor::Attributes = {
		{
			.location = 0,
			.binding = 0,
			.format = VK_FORMAT_R32G32_SFLOAT,
			.offset = offsetof(VertexPosColor, pos)
		},
		{
			.location = 1,
			.binding = 0,
			.format = VK_FORMAT_R32G32B32_SFLOAT,
			.offset = offsetof(VertexPosColor, color)
		}
	};

	const VkVertexInputBindingDescription VertexPosColorUv::Binding = {
		.binding = 0,
		.stride = sizeof(VertexPosColorUv),
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
	};

	const std::vector<VkVertexInputAttributeDescription> VertexPosColorUv::Attributes = {
		{
			.location = 0,
			.binding = 0,
			.format = VK_FORMAT_R32G32B32_SFLOAT,
			.offset = offsetof(VertexPosColorUv, pos)
		},
		{
			.location = 1,
			.binding = 0,
			.format = VK_FORMAT_R32G32B32_SFLOAT,
			.offset = offsetof(VertexPosColorUv, color)
		},
		{
			.location = 2,
			.binding = 0,
			.format = VK_FORMAT_R32G32_SFLOAT,
			.offset = offsetof(VertexPosColorUv, uv)
		}
	};
}