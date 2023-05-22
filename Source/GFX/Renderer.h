#pragma once

#include "RenderPass.h"
#include "RenderTarget.h"
#include "TriangleRenderer.h"

namespace gfx {

	class Renderer {
	public:
		Renderer();
		~Renderer();

		void Init();
		void Destroy();

		VkCommandBuffer Begin();
		void Composite(VkCommandBuffer commandBuffer);
		void End(VkCommandBuffer commandBuffer);

		void Resized();

		inline f32 Aspect() const { return extent.x / static_cast<f32>(extent.y); }

	private:
		std::unordered_map<std::string, std::unique_ptr<RenderPass>> passes;

		std::unordered_map<std::string, std::unique_ptr<RenderTarget>> targets;

		std::vector<VkCommandBuffer> commandBuffers;

		bool frameStarted = false;

		uvec2 extent;
		uint32_t imageIndex;

		PipelineCache cache;

		TriangleRenderer triRenderer;
	};
}