#include "Swapchain.h"
#include "Util\Configuration.h"
#include "State.h"

namespace platform {

	static VkExtent2D PickExtent(const Device::SwapchainCapabilities& caps, const Window& window) {
		if (caps.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return caps.capabilities.currentExtent;
		}
		else {
			VkExtent2D extent = vk::VecToExtent(window.FullSize());
			extent.width = math::Clamp(extent.width,
				caps.capabilities.minImageExtent.width,
				caps.capabilities.maxImageExtent.width);
			extent.height = math::Clamp(extent.height,
				caps.capabilities.minImageExtent.height,
				caps.capabilities.maxImageExtent.height);

			return extent;
		}
	}

	static VkSurfaceFormatKHR PickSurfaceFormat(const Device::SwapchainCapabilities& caps) {
		for (const auto& format : caps.formats) {
			if (format.format == VK_FORMAT_B8G8R8A8_SRGB
				&& format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return format;
			}
		}

		return caps.formats[0]; //TODO
	}

	static VkPresentModeKHR PickPresentMode(const Device::SwapchainCapabilities& caps) {
		if (global.config->vsync) {
			return VK_PRESENT_MODE_FIFO_KHR;
		}

		for (const auto& mode : caps.presentModes) {
			if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return VK_PRESENT_MODE_MAILBOX_KHR;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	Swapchain::Swapchain(const Device& device, const Window& window)
		: device(device), window(window) {
		CreateSwapchain();
		CreateImageViews();

		inFlightFences.resize(vk::MAX_FRAMES_IN_FLIGHT);
		imageAvailableSemaphores.resize(vk::MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(vk::MAX_FRAMES_IN_FLIGHT);
		for (uint32_t i = 0; i < vk::MAX_FRAMES_IN_FLIGHT; i++) {
			VkFenceCreateInfo fenceInfo{};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			VkSemaphoreCreateInfo semInfo{};
			semInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			VULKAN_CHECK(vkCreateFence(
				device,
				&fenceInfo,
				nullptr,
				&inFlightFences[i]
			), "Failed to create swapchain fence!");

			VULKAN_CHECK(vkCreateSemaphore(
				device,
				&semInfo,
				nullptr,
				&imageAvailableSemaphores[i]
			), "Failed to create swapchain semaphore!");

			VULKAN_CHECK(vkCreateSemaphore(
				device,
				&semInfo,
				nullptr,
				&renderFinishedSemaphores[i]
			), "Failed to create swapchain semaphore!");
		}
	}

	Swapchain::~Swapchain() {
		for (auto fence : inFlightFences) {
			vkDestroyFence(device, fence, nullptr);
		}

		for (auto sem : imageAvailableSemaphores) {
			vkDestroySemaphore(device, sem, nullptr);
		}

		for (auto sem : renderFinishedSemaphores) {
			vkDestroySemaphore(device, sem, nullptr);
		}

		for (auto view : imageViews) {
			vkDestroyImageView(device, view, nullptr);
		}

		vkDestroySwapchainKHR(device, swapchain, nullptr);
	}

	void Swapchain::LogInfo() const {
		LOGNFNG(util::Logger::Vulkan,
			"Created swapchain with present mode $, format $, color space $, and extent $.",
			vk::ToString(presentMode),
			vk::ToString(format.format),
			vk::ToString(format.colorSpace),
			vk::ExtentToVec(extent));
	}

	void Swapchain::CreateSwapchain(VkSwapchainKHR oldSwapchain) {
		//Figure out details
		auto caps = device.GetSwapchainCapabilities();
		extent = PickExtent(caps, window);
		format = PickSurfaceFormat(caps);
		presentMode = PickPresentMode(caps);

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = device.Surface();
		createInfo.oldSwapchain = oldSwapchain;
		
		auto indices = device.GetQueueFamilyIndices();
		std::vector<uint32_t> uniqueIndices = { *indices.graphicsFamily, *indices.presentFamily };
		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = static_cast<uint32_t>(uniqueIndices.size());
			createInfo.pQueueFamilyIndices = uniqueIndices.data();
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
		}

		createInfo.clipped = VK_TRUE;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.preTransform = caps.capabilities.currentTransform;
		createInfo.presentMode = presentMode;

		createInfo.imageArrayLayers = 1;
		createInfo.imageColorSpace = format.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageFormat = format.format;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.minImageCount = caps.capabilities.minImageCount + 1;
		if (caps.capabilities.minImageCount != 0 && createInfo.minImageCount > caps.capabilities.maxImageCount) {
			createInfo.minImageCount = caps.capabilities.maxImageCount;
		}

		VULKAN_CHECK(vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain), "Failed to create swapchain!");

		uint32_t count;
		vkGetSwapchainImagesKHR(device, swapchain, &count, nullptr);
		images.resize(count);
		vkGetSwapchainImagesKHR(device, swapchain, &count, images.data());
	}

	void Swapchain::CreateImageViews() {
		imageViews.resize(ImageCount());
		for (uint32_t i = 0; i < images.size(); i++) {
			VULKAN_CHECK(
				device.CreateImageView(
					images[i],
					format.format,
					VK_IMAGE_VIEW_TYPE_2D,
					1, 1,
					VK_IMAGE_ASPECT_COLOR_BIT,
					imageViews[i]
				), "Failed to create swapchain image views!");
		}
	}

	VkResult Swapchain::AcquireImage(uint32_t* imageIndex) {
		vkWaitForFences(
			device,
			1,
			&inFlightFences[frameIndex],
			VK_TRUE,
			UINT64_MAX
		);

		vkResetFences(device, 1, &inFlightFences[frameIndex]);

		return vkAcquireNextImageKHR(
			device,
			swapchain,
			UINT64_MAX,
			imageAvailableSemaphores[frameIndex],
			VK_NULL_HANDLE,
			imageIndex
		);
	}

	VkResult Swapchain::SubmitFrame(VkCommandBuffer commandBuffer, uint32_t* imageIndex) {
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &imageAvailableSemaphores[frameIndex];

		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &renderFinishedSemaphores[frameIndex];

		VULKAN_CHECK(vkQueueSubmit(device.GraphicsQueue(), 1, &submitInfo, inFlightFences[frameIndex]),
			"Failed to submit draw command buffer!");

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapchain;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &renderFinishedSemaphores[frameIndex];
		presentInfo.pImageIndices = imageIndex;
		
		VkResult result = vkQueuePresentKHR(device.PresentQueue(), &presentInfo);

		frameIndex = (frameIndex + 1) % vk::MAX_FRAMES_IN_FLIGHT;

		return result;
	}

	void Swapchain::Recreate() {
		VkSwapchainKHR oldSwapchain = swapchain;
		VkSurfaceFormatKHR oldFormat = format;

		CreateSwapchain(oldSwapchain);

		//TODO: will this ever actually happen?
		ASSERT(oldFormat == format, "Swapchain formats have changed!"); //TODO: somehow signal that formats have changed instead of just crashing

		vkDestroySwapchainKHR(device, oldSwapchain, nullptr);

		for (auto view : imageViews) {
			vkDestroyImageView(device, view, nullptr);
		}

		CreateImageViews();
	}
}