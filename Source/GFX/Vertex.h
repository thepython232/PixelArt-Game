#pragma once

#include "Util\Types.h"
#include "Util\Std.h"
#include "Util\Vulkan.h"
#include "Util\Math.h"

namespace gfx {

	template<typename T>
	concept IsVertex = requires {
		requires std::same_as<decltype(T::Binding), const VkVertexInputBindingDescription>;
		requires std::same_as<decltype(T::Attributes), const std::vector<VkVertexInputAttributeDescription>>;
	};

	struct VertexPosColor {
		vec2 pos;
		vec3 color;

		static const VkVertexInputBindingDescription Binding;

		static const std::vector<VkVertexInputAttributeDescription> Attributes;
	};

	struct VertexPosColorUv {
		vec3 pos;
		vec3 color;
		vec2 uv;

		static const VkVertexInputBindingDescription Binding;

		static const std::vector<VkVertexInputAttributeDescription> Attributes;
	};
}