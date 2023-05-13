#include "TriangleRenderer.h"

namespace gfx {

	void TriangleRenderer::Init(PipelineCache* cache, VkRenderPass renderPass) {
		Pipeline::LayoutSettings layoutSettings;

		Shader vertShader{ "Shaders\\simple.vert.spv", VK_SHADER_STAGE_VERTEX_BIT };
		Shader fragShader{ "Shaders\\simple.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT };

		auto settings = GraphicsPipeline::DefaultSettings(vertShader, fragShader, renderPass);

		pipeline = std::make_unique<GraphicsPipeline>(layoutSettings, settings, cache);
	}

	void TriangleRenderer::Render(VkCommandBuffer commandBuffer) {
		pipeline->Bind(commandBuffer);
		vkCmdDraw(commandBuffer, 3, 1, 0, 0);
	}
}