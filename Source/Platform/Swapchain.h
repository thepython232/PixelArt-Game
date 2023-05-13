#pragma once

#include "Device.h"

namespace platform {

	class Swapchain {
	public:
		Swapchain(const Device& device, const Window& window);
		~Swapchain();

		void LogInfo() const;

		inline operator VkSwapchainKHR() const { return swapchain; }
		inline VkSwapchainKHR GetSwapchain() const { return swapchain; }
		inline VkFormat Format() const { return format.format; }
		inline VkColorSpaceKHR ColorSpace() const { return format.colorSpace; }
		inline VkPresentModeKHR PresentMode() const { return presentMode; }
		inline VkExtent2D Extent() const { return extent; }
		inline VkImage GetImage(uint32_t index) const { return images[index]; }
		inline VkImageView GetImageView(uint32_t index) const { return imageViews[index]; }
		inline uint32_t CurrentFrame() const { return frameIndex; }
		inline uint32_t ImageCount() const { return static_cast<uint32_t>(images.size()); }

		VkResult AcquireImage(uint32_t* imageIndex);
		VkResult SubmitFrame(VkCommandBuffer commandBuffer, uint32_t* imageIndex);

		void Recreate();

	private:
		void CreateSwapchain(VkSwapchainKHR oldSwapchain = VK_NULL_HANDLE);
		void CreateImageViews();
		
		VkSwapchainKHR swapchain;
		VkSurfaceFormatKHR format;
		VkExtent2D extent;
		VkPresentModeKHR presentMode;

		std::vector<VkImage> images;
		std::vector<VkImageView> imageViews;

		std::vector<VkFence> inFlightFences;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;

		uint32_t frameIndex = 0;

		const Device& device;
		const Window& window;
	};
}