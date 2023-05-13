#include "RenderPass.h"
#include "Platform\Platform.h"
#include "State.h"

namespace gfx {

	RenderPass::Builder& RenderPass::Builder::AddAttachment(
		const RenderTarget& target,
		std::initializer_list<std::pair<uint32_t, RenderTarget::Type>>&& usage,
		u32 flags,
		VkClearValue clear,
		VkImageLayout initialLayout,
		VkImageLayout finalLayout
	) {
		VkImageLayout layoutType = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		if (vk::IsDepthFormat(target.GetFormat()) || vk::IsStencilFormat(target.GetFormat())) {
			layoutType = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		}

		VkAttachmentDescription attachment{};
		attachment.format = target.GetFormat();
		attachment.samples = target.NumSamples();
		if (flags & Flags::Clear) {
			attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		}
		else if (flags & Flags::Load) {
			attachment.initialLayout = (initialLayout == VK_IMAGE_LAYOUT_UNDEFINED) ? layoutType : initialLayout;
			attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		}
		else {
			attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		}

		if (flags & Flags::Store) {
			attachment.finalLayout = (finalLayout == VK_IMAGE_LAYOUT_UNDEFINED) ? layoutType : finalLayout;
			attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		}
		else {
			attachment.finalLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		}

		for (const auto& [subpass, type] : usage) {
			VkAttachmentReference reference{};
			reference.attachment = static_cast<uint32_t>(attachments.size());
			reference.layout = layoutType;
			switch (type)
			{
			case RenderTarget::Color:
				subpasses[subpass].colorRefs.push_back(reference);
				break;
			case RenderTarget::Resolve:
				subpasses[subpass].resolveRefs.push_back(reference);
				break;
			case RenderTarget::Input:
				subpasses[subpass].inputRefs.push_back(reference);
				break;
			case RenderTarget::Depth:
				if (subpasses[subpass].depthRef)
					WARN("Subpass already has depth reference!");
				subpasses[subpass].depthRef = reference;
				break;
			}
		}

		Attachment attach{};
		attach.description = attachment;
		attach.target = &target;

		attachments.push_back(attach);

		return *this;
	}

	RenderPass::Builder& RenderPass::Builder::AddSwapAttachment(
		std::initializer_list<std::pair<uint32_t, RenderTarget::Type>>&& usage,
		u32 flags,
		VkClearColorValue clear
	) {
		VkAttachmentDescription description{};
		description.format = global.platform->swapchain->Format();
		description.samples = VK_SAMPLE_COUNT_1_BIT;
		description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

		description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		if (flags & Flags::Load) {
			description.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
			description.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}
		else if (flags & Flags::Clear) {
			description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		}
		else {
			description.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		}

		if (flags & Flags::Presentable) {
			description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		}
		else {
			description.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}

		for (const auto& [subpass, type] : usage) {
			VkAttachmentReference reference{};
			reference.attachment = static_cast<uint32_t>(attachments.size());
			reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			switch (type)
			{
			case RenderTarget::Color:
				subpasses[subpass].colorRefs.push_back(reference);
				break;
			case RenderTarget::Resolve:
				subpasses[subpass].resolveRefs.push_back(reference);
				break;
			case RenderTarget::Input:
				subpasses[subpass].inputRefs.push_back(reference);
				break;
			default:
				WARN("Invalid swapchain attachment usage!");
			}
		}

		Attachment attach{};
		attach.clear = VkClearValue{ clear };
		attach.description = description;
		attach.target = nullptr;

		attachments.push_back(attach);

		return *this;
	}

	RenderPass::Builder& RenderPass::Builder::AddDependency(
		uint32_t srcSubpass,
		uint32_t dstSubpass,
		VkPipelineStageFlags srcStage,
		VkPipelineStageFlags dstStage,
		VkAccessFlags srcAccess,
		VkAccessFlags dstAccess,
		VkDependencyFlags flags
	) {
		VkSubpassDependency dependency{};
		dependency.srcSubpass = srcSubpass;
		dependency.dstSubpass = dstSubpass;
		dependency.srcAccessMask = srcAccess;
		dependency.dstAccessMask = dstAccess;
		dependency.srcStageMask = srcStage;
		dependency.dstStageMask = dstStage;
		dependency.dependencyFlags = flags;

		dependencies.push_back(dependency);

		return *this;
	}

	std::unique_ptr<RenderPass> RenderPass::Builder::Create() {
		return std::make_unique<RenderPass>(extent, subpasses, attachments, dependencies);
	}

	RenderPass::RenderPass(
		VkExtent2D extent,
		const std::vector<Subpass>& subpasses,
		const std::vector<Attachment>& attachments,
		const std::vector<VkSubpassDependency>& dependencies
	) : extent(extent) {
		std::vector<VkAttachmentDescription> descriptions;
		for (const auto& attach : attachments) {
			descriptions.push_back(attach.description);
			clearValues.push_back(attach.clear);
			targets.push_back(attach.target);
		}

		std::vector<VkSubpassDescription> subpassStructs;
		for (const auto& subpass : subpasses) {
			VkSubpassDescription subpassStruct{};
			subpassStruct.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpassStruct.colorAttachmentCount = static_cast<uint32_t>(subpass.colorRefs.size());
			subpassStruct.pColorAttachments = subpass.colorRefs.data();
			subpassStruct.inputAttachmentCount = static_cast<uint32_t>(subpass.inputRefs.size());
			subpassStruct.pInputAttachments = subpass.inputRefs.data();
			subpassStruct.pResolveAttachments = subpass.resolveRefs.data();
			if (subpass.depthRef) {
				subpassStruct.pDepthStencilAttachment = &subpass.depthRef.value();
			}

			subpassStructs.push_back(subpassStruct);
		}

		VkRenderPassCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.attachmentCount = static_cast<uint32_t>(descriptions.size());
		createInfo.pAttachments = descriptions.data();
		createInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		createInfo.pDependencies = dependencies.data();
		createInfo.subpassCount = static_cast<uint32_t>(subpassStructs.size());
		createInfo.pSubpasses = subpassStructs.data();

		VULKAN_CHECK(vkCreateRenderPass(*global.platform->device, &createInfo, nullptr, &renderPass),
			"Failed to create render pass!");

		framebuffers.resize(global.platform->swapchain->ImageCount());
		for (uint32_t i = 0; i < framebuffers.size(); i++) {
			std::vector<VkImageView> views;
			for (const auto& attachment : attachments) {
				if (attachment.target) {
					views.push_back(attachment.target->GetView(i));
				}
				else {
					views.push_back(global.platform->swapchain->GetImageView(i));
				}
			}

			VkFramebufferCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfo.renderPass = renderPass;
			createInfo.width = extent.width;
			createInfo.height = extent.height;
			createInfo.layers = 1;
			createInfo.attachmentCount = static_cast<uint32_t>(views.size());
			createInfo.pAttachments = views.data();

			VULKAN_CHECK(vkCreateFramebuffer(*global.platform->device, &createInfo, nullptr, &framebuffers[i]),
				"Failed to create framebuffer!");
		}
	}

	RenderPass::~RenderPass() {
		for (auto framebuffer : framebuffers) {
			vkDestroyFramebuffer(*global.platform->device, framebuffer, nullptr);
		}

		vkDestroyRenderPass(*global.platform->device, renderPass, nullptr);
	}

	void RenderPass::Begin(VkCommandBuffer commandBuffer, uint32_t imageIndex) const {
		VkRenderPassBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		beginInfo.renderPass = renderPass;
		beginInfo.renderArea.offset = { 0, 0 };
		beginInfo.renderArea.extent = extent;
		beginInfo.framebuffer = framebuffers[imageIndex];
		beginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		beginInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0;
		viewport.y = 0;
		viewport.width = static_cast<float>(extent.width);
		viewport.height = static_cast<float>(extent.height);
		viewport.minDepth = 0.f;
		viewport.maxDepth = 1.f;

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = extent;

		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void RenderPass::Recreate(VkExtent2D newExtent) {
		extent = newExtent;

		for (auto framebuffer : framebuffers) {
			vkDestroyFramebuffer(*global.platform->device, framebuffer, nullptr);
		}

		for (uint32_t i = 0; i < framebuffers.size(); i++) {
			std::vector<VkImageView> views;
			for (const auto& target : targets) {
				if (target) {
					views.push_back(target->GetView(i));
				}
				else {
					views.push_back(global.platform->swapchain->GetImageView(i));
				}
			}

			VkFramebufferCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfo.renderPass = renderPass;
			createInfo.width = extent.width;
			createInfo.height = extent.height;
			createInfo.layers = 1;
			createInfo.attachmentCount = static_cast<uint32_t>(views.size());
			createInfo.pAttachments = views.data();

			VULKAN_CHECK(vkCreateFramebuffer(*global.platform->device, &createInfo, nullptr, &framebuffers[i]),
				"Failed to create framebuffer!");
		}
	}
}