#pragma once

#include "Instance.h"
#include "Window.h"

namespace platform {

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	class Device {
	public:
		struct QueueFamilyIndices {
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;
			std::optional<uint32_t> computeFamily;

			inline bool IsComplete() {
				return graphicsFamily.has_value() && presentFamily.has_value() && computeFamily.has_value();
			}
		};

		struct SwapchainCapabilities {
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkPresentModeKHR> presentModes;
			std::vector<VkSurfaceFormatKHR> formats;
		};

		Device(const Instance& instance, const Window& window);
		~Device();

		Device(const Device& other) = delete;
		Device& operator=(const Device& other) = delete;
		Device(Device&& other) noexcept = delete;
		Device& operator=(Device&& other) noexcept = delete;

		inline operator VkDevice() const { return device; }
		inline VkDevice GetDevice() const { return device; }
		inline VkPhysicalDevice PhysicalDevice() const { return physicalDevice; }
		inline VkSurfaceKHR Surface() const { return surface; }
		inline VkQueue GraphicsQueue() const { return graphicsQueue; }
		inline VkQueue PresentQueue() const { return presentQueue; }
		inline VkQueue ComputeQueue() const { return computeQueue; }
		inline VkCommandPool GraphicsPool() const { return graphicsPool; }
		inline VkCommandPool ComputePool() const { return computePool; }

		VkPhysicalDeviceProperties Properties() const;
		VkPhysicalDeviceFeatures Features() const;
		VkPhysicalDeviceLimits Limits() const {
			return Properties().limits;
		}

		void LogInfo() const;

		VkResult CreateBuffer(
			VkDeviceSize size,
			VkBufferUsageFlags usage,
			VkMemoryPropertyFlags memProps,
			uint32_t families,
			VkBuffer& buffer,
			VkDeviceMemory& memory
		) const;

		VkResult CreateImage(
			VkFormat format,
			VkExtent3D extent,
			VkImageType type,
			uint32_t levels, uint32_t layers,
			VkImageTiling tiling,
			VkImageLayout initialLayout,
			VkSampleCountFlagBits samples,
			VkImageUsageFlags usage,
			VkMemoryPropertyFlags memProps,
			uint32_t families,
			VkImage& image,
			VkDeviceMemory& memory
		) const;

		VkResult CreateImageView(
			VkImage image,
			VkFormat format,
			VkImageViewType type,
			uint32_t levels, uint32_t layers,
			VkImageAspectFlags aspect,
			VkImageView& view
		) const;

		VkCommandBuffer BeginSingleTime() const;
		VkCommandBuffer BeginSingleTimeCompute() const;
		void EndSingleTime(VkCommandBuffer commandBuffer, VkFence* fence = VK_NULL_HANDLE) const;
		void EndSingleTimeCompute(VkCommandBuffer commandBuffer, VkFence* fence = VK_NULL_HANDLE) const;

		void CopyBuffer(
			VkBuffer src,
			VkBuffer dst,
			VkDeviceSize size,
			VkDeviceSize srcOffset = 0,
			VkDeviceSize dstOffset = 0
		) const;

		void CopyBufferToImage(
			VkBuffer src,
			VkImage dst,
			VkExtent3D extent,
			VkImageAspectFlags aspect,
			uint32_t level = 0
		) const;

		void CopyImageToBuffer(
			VkImage src,
			VkBuffer dst,
			VkExtent3D extent,
			VkImageAspectFlags aspect,
			uint32_t level = 0
		) const;

		void CopyImage(
			VkImage src,
			VkImage dst,
			VkExtent3D extent,
			VkImageAspectFlags aspect,
			uint32_t srcLevel = 0,
			uint32_t dstLevel = 0
		) const;

		inline QueueFamilyIndices GetQueueFamilyIndices() const {
			ASSERT(physicalDevice != VK_NULL_HANDLE, "No physical device yet!");
			return GetQueueFamilyIndices(physicalDevice);
		}

		inline SwapchainCapabilities GetSwapchainCapabilities() const {
			ASSERT(physicalDevice != VK_NULL_HANDLE, "No physical device yet!");
			return GetSwapchainCapabilities(physicalDevice);
		}

		uint32_t FindMemoryType(const VkMemoryRequirements& memReqs, VkMemoryPropertyFlags memProps) const;

	private:
		QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice device) const;

		SwapchainCapabilities GetSwapchainCapabilities(VkPhysicalDevice device) const;

		VkDevice device;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkSurfaceKHR surface;

		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkQueue computeQueue;

		VkCommandPool graphicsPool;
		VkCommandPool computePool;

		const Instance& instance;
	};
}