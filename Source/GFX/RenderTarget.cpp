#include "RenderTarget.h"
#include "Platform\Platform.h"
#include "State.h"

namespace gfx {

	usize RenderTarget::nextID = 0;

	RenderTarget::RenderTarget(
		VkFormat format,
		VkExtent2D extent,
		VkSampleCountFlagBits samples,
		b8 sampled
	) : format(format), samples(samples) {
		uid = nextID++;

		VkImageUsageFlags usage = sampled ? VK_IMAGE_USAGE_SAMPLED_BIT : 0;
		if (vk::IsColorFormat(format)) {
			usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		}
		else {
			usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		}

		images.resize(global.platform->swapchain->ImageCount());
		views.resize(global.platform->swapchain->ImageCount());
		memory.resize(global.platform->swapchain->ImageCount());
		for (u32 i = 0; i < images.size(); i++) {
			VULKAN_CHECK(global.platform->device->CreateImage(
				format,
				vk::Extent2DTo3D(extent),
				VK_IMAGE_TYPE_2D,
				1, 1,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_LAYOUT_UNDEFINED,
				samples,
				usage,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				vk::QueueFamilies::Graphics,
				images[i],
				memory[i]
			), "Failed to create render texture!");

			VULKAN_CHECK(global.platform->device->CreateImageView(
				images[i],
				format,
				VK_IMAGE_VIEW_TYPE_2D,
				1, 1,
				vk::AspectFromFormat(format),
				views[i]
			), "Failed to create target image view!");
		}
	}

	RenderTarget::~RenderTarget() {
		for (u32 i = 0; i < images.size(); i++) {
			vkDestroyImageView(*global.platform->device, views[i], nullptr);
			vkDestroyImage(*global.platform->device, images[i], nullptr);
			vkFreeMemory(*global.platform->device, memory[i], nullptr);
		}
	}

	VkDescriptorImageInfo RenderTarget::GetDescriptorInfo(uint32_t index) const {
		VkDescriptorImageInfo info{};
		info.imageView = views[index];
		if (vk::IsColorFormat(format)) {
			info.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}
		else {
			info.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		}
		return info;
	}

	void RenderTarget::Resize(VkExtent2D newExtent) {
		for (u32 i = 0; i < images.size(); i++) {
			vkDestroyImageView(*global.platform->device, views[i], nullptr);
			vkDestroyImage(*global.platform->device, images[i], nullptr);
			vkFreeMemory(*global.platform->device, memory[i], nullptr);
		}

		VkImageUsageFlags usage = VK_IMAGE_USAGE_SAMPLED_BIT;
		if (vk::IsColorFormat(format)) {
			usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		}
		else {
			usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		}

		images.resize(global.platform->swapchain->ImageCount());
		views.resize(global.platform->swapchain->ImageCount());
		memory.resize(global.platform->swapchain->ImageCount());
		for (u32 i = 0; i < images.size(); i++) {
			VULKAN_CHECK(global.platform->device->CreateImage(
				format,
				vk::Extent2DTo3D(newExtent),
				VK_IMAGE_TYPE_2D,
				1, 1,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_LAYOUT_UNDEFINED,
				samples,
				usage,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				vk::QueueFamilies::Graphics,
				images[i],
				memory[i]
			), "Failed to create render texture!");

			VULKAN_CHECK(global.platform->device->CreateImageView(
				images[i],
				format,
				VK_IMAGE_VIEW_TYPE_2D,
				1, 1,
				vk::AspectFromFormat(format),
				views[i]
			), "Failed to create target image view!");
		}
	}
}