#pragma once

#include "RenderTarget.h"

namespace gfx {

	class RenderPass {
	public:
		struct Subpass {
			std::vector<VkAttachmentReference> colorRefs;
			std::vector<VkAttachmentReference> resolveRefs;
			std::vector<VkAttachmentReference> inputRefs;
			std::optional<VkAttachmentReference> depthRef;
		};

		struct Attachment {
			VkAttachmentDescription description;
			const RenderTarget* target;
			VkClearValue clear;
		};

		struct Builder {
			enum Flags : i32 {
				Load = 1 << 0,
				Store = 1 << 1,
				Clear = 1 << 2,
				Presentable = 1 << 3
			};

			Builder(VkExtent2D extent, uint32_t numSubpasses)
				: extent(extent) {
				subpasses.resize(numSubpasses);
			}

			Builder& AddAttachment(
				const RenderTarget& target,
				std::initializer_list<std::pair<uint32_t, RenderTarget::Type>>&& usage,
				u32 flags,
				VkClearValue clear,
				VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				VkImageLayout finalLayout = VK_IMAGE_LAYOUT_UNDEFINED
			);

			Builder& AddSwapAttachment(
				std::initializer_list<std::pair<uint32_t, RenderTarget::Type>>&& usage,
				u32 flags,
				VkClearColorValue clear
			);

			Builder& AddDependency(
				uint32_t srcSubpass,
				uint32_t dstSubpass,
				VkPipelineStageFlags srcStage,
				VkPipelineStageFlags dstStage,
				VkAccessFlags srcAccess,
				VkAccessFlags dstAccess,
				VkDependencyFlags flags = 0
			);

			std::unique_ptr<RenderPass> Create();

		private:
			VkExtent2D extent;

			std::vector<Subpass> subpasses;
			std::vector<Attachment> attachments;
			std::vector<VkSubpassDependency> dependencies;
		};

		RenderPass(
			VkExtent2D extent,
			const std::vector<Subpass>& subpasses,
			const std::vector<Attachment>& attachments,
			const std::vector<VkSubpassDependency>& dependencies
		);

		~RenderPass();

		inline operator VkRenderPass() const { return renderPass; }
		inline VkRenderPass GetRenderPass() const { return renderPass; }

		void Begin(VkCommandBuffer commandBuffer, uint32_t imageIndex) const;
		void Recreate(VkExtent2D newExtent);

	private:
		VkRenderPass renderPass;
		std::vector<VkFramebuffer> framebuffers;
		VkExtent2D extent;
		std::vector<VkClearValue> clearValues;
		std::vector<const RenderTarget*> targets;
	};
}