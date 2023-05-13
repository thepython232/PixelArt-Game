#include "Renderer.h"
#include "Platform\Platform.h"
#include "Util\Configuration.h"
#include "State.h"

namespace gfx {

	Renderer::Renderer() : extent(vk::ExtentToVec(global.platform->swapchain->Extent())), cache(global.config->pipelineCache) {
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = global.platform->device->GraphicsPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = vk::MAX_FRAMES_IN_FLIGHT;
		commandBuffers.resize(vk::MAX_FRAMES_IN_FLIGHT);

		VULKAN_CHECK(vkAllocateCommandBuffers(
			*global.platform->device,
			&allocInfo,
			commandBuffers.data()
		), "Failed to allocate command buffers!");
	}

	Renderer::~Renderer() {
		vkFreeCommandBuffers(
			*global.platform->device,
			global.platform->device->GraphicsPool(),
			vk::MAX_FRAMES_IN_FLIGHT,
			commandBuffers.data()
		);
	}

	void Renderer::Init() {
		passes["Main"] = RenderPass::Builder(vk::VecToExtent(extent), 1)
			.AddSwapAttachment(
				{ {0, RenderTarget::Color} },
				RenderPass::Builder::Clear | RenderPass::Builder::Presentable,
				{ {0.f, 0.f, 0.f} })
			.Create();

		triRenderer.Init(&cache, *passes["Main"]);
	}

	void Renderer::Destroy() {

	}

	VkCommandBuffer Renderer::Begin() {
		ASSERT(!frameStarted, "Can't call Renderer::Begin when the frame is already running!");

		frameStarted = true;

		VkResult result = global.platform->swapchain->AcquireImage(&imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			Resized();
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			ERROR((i32)result, util::Logger::Vulkan, "Failed to acquire swapchain image!");
		}

		//VULKAN_CHECK(vkResetCommandBuffer(
		//	commandBuffers[global.platform->swapchain->CurrentFrame()], 0),
		//	"Failed to reset command buffer!");

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		
		VULKAN_CHECK(vkBeginCommandBuffer(
			commandBuffers[global.platform->swapchain->CurrentFrame()],
			&beginInfo),
			"Failed to begin command buffer!");

		return commandBuffers[global.platform->swapchain->CurrentFrame()];
	}

	void Renderer::Composite(VkCommandBuffer commandBuffer) {
		ASSERT(frameStarted, "Can't composite frame when it hasn't started yet!");

		passes["Main"]->Begin(commandBuffer, imageIndex);

		//TODO: draw here
		triRenderer.Render(commandBuffer);

		vkCmdEndRenderPass(commandBuffer);
	}

	void Renderer::End(VkCommandBuffer commandBuffer) {
		ASSERT(frameStarted, "Can't end frame when it hasn't started yet!");

		VULKAN_CHECK(vkEndCommandBuffer(commandBuffer),
			"Failed to end command buffer!");

		VkResult result = global.platform->swapchain->SubmitFrame(commandBuffer, &imageIndex);

		if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR) {
			Resized();
		}
		else if (result != VK_SUCCESS) {
			ERROR((i32)result, util::Logger::Vulkan, "Failed to acquire image!");
		}

		frameStarted = false;
	}

	void Renderer::Resized() {
		while (global.platform->window->IsMinimized());

		vkDeviceWaitIdle(*global.platform->device);

		global.platform->swapchain->Recreate();

		extent = static_cast<uvec2>(vk::ExtentToVec(global.platform->swapchain->Extent()));

		LOG("Window resized to $.", extent);

		for (auto& [name, pass] : passes) {
			pass->Recreate(vk::VecToExtent(extent));
		}
	}
}