#pragma once

#include "GraphicsPipeline.h"
#include "Buffer.h"
#include "Descriptors.h"
#include "Texture.h"

namespace gfx {

	class TriangleRenderer {
	public:
		void Init(PipelineCache* cache, VkRenderPass renderPass);

		void Render(VkCommandBuffer commandBuffer, uint32_t frameIndex);

	private:
		std::unique_ptr<GraphicsPipeline> pipeline;
		std::unique_ptr<Buffer> vbuffer;
		std::unique_ptr<DescriptorPool> pool;
		std::unique_ptr<DescriptorSetLayout> layout;
		std::vector<std::unique_ptr<Buffer>> uniforms;
		std::vector<VkDescriptorSet> sets;
		std::unique_ptr<Texture> texture;
	};
}