#include "ComputePipeline.h"
#include "Platform\Platform.h"
#include "State.h"

namespace gfx {

	ComputePipeline::ComputePipeline(const LayoutSettings& layoutSettings,
		const Shader& shader, PipelineCache* cache)
		: Pipeline(layoutSettings, cache) {
		VkComputePipelineCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		createInfo.layout = layout;
		createInfo.stage = shader.Create();

		VULKAN_CHECK(vkCreateComputePipelines(
			*global.platform->device,
			cache ? cache->GetCache() : VK_NULL_HANDLE,
			1, &createInfo, nullptr, &pipeline),
			"Failed to create graphics pipeline!");
	}
}