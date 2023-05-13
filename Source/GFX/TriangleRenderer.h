#pragma once

#include "GraphicsPipeline.h"

namespace gfx {

	class TriangleRenderer {
	public:
		void Init(PipelineCache* cache, VkRenderPass renderPass);

		void Render(VkCommandBuffer commandBuffer);

	private:
		std::unique_ptr<GraphicsPipeline> pipeline;
	};
}