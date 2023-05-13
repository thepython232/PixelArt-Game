#pragma once

#include "Util\Types.h"
#include "Util\Std.h"
#include "Util\Vulkan.h"
#include "Util\Math.h"

namespace gfx {

	template<typename T>
	concept IsVertex = requires {
		requires std::same_as<decltype(T::Binding), VkVertexInputBindingDescription>;
		requires std::same_as<decltype(T::Attributes), std::vector<VkVertexInputAttributeDescription>>;
	};

	struct VertexPosColor {
		vec2 pos;
		vec3 color;

		static VkVertexInputBindingDescription Binding;

		static std::vector<VkVertexInputAttributeDescription> Attributes;
	};
}