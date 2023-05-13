#include "Device.h"

static std::vector<uint32_t> UniqueQueueFamilies(const platform::Device::QueueFamilyIndices& indices, platform::Device::QueueFamilyIndices::Type types) {
	std::vector<uint32_t> families;

	if (types & platform::Device::QueueFamilyIndices::Graphics) {
		families.push_back(*indices.graphicsFamily);
	}
	if ((types & platform::Device::QueueFamilyIndices::Present)
		&& indices.graphicsFamily != indices.presentFamily) {
		families.push_back(*indices.presentFamily);
	}
	if ((types & platform::Device::QueueFamilyIndices::Compute)
		&& indices.graphicsFamily != indices.computeFamily
		&& indices.presentFamily != indices.computeFamily) {
		families.push_back(*indices.computeFamily);
	}

	return families;
}

static bool IsDeviceSuitable(VkPhysicalDevice device) {
	VkPhysicalDeviceProperties props;
	vkGetPhysicalDeviceProperties(device, &props);
	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(device, &features);

	return true; //TODO
}

static uint32_t DeviceScore(VkPhysicalDevice device) {
	VkPhysicalDeviceProperties props;
	vkGetPhysicalDeviceProperties(device, &props);
	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(device, &features);

	uint32_t score = 0;

	if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		score += 1000;
	}

	score += props.limits.maxImageDimension2D;

	if (features.samplerAnisotropy) {
		score += 200;
	}

	return score;
}

static VkPhysicalDevice PickPhysicalDevice(VkInstance instance) {
	uint32_t count;
	vkEnumeratePhysicalDevices(instance, &count, nullptr);
	std::vector<VkPhysicalDevice> devices(count);
	vkEnumeratePhysicalDevices(instance, &count, devices.data());

	std::map<uint32_t, VkPhysicalDevice, std::greater<uint32_t>> deviceScores;
	for (const auto& device : devices) {
		if (IsDeviceSuitable(device)) {
			uint32_t score = DeviceScore(device);
			deviceScores.insert(std::make_pair(score, device));
		}
	}

	if (deviceScores.empty()) {
		return VK_NULL_HANDLE;
	}
	else {
		return deviceScores.begin()->second;
	}
}

namespace platform {

	Device::Device(const Instance& instance, const Window& window)
		: instance(instance) {
		VULKAN_CHECK(window.CreateSurface(instance, nullptr, &surface),
			"Failed to create window surface!");

		physicalDevice = PickPhysicalDevice(instance);

		ASSERT(physicalDevice != VK_NULL_HANDLE, "Failed to find a suitable GPU!");

		VkPhysicalDeviceFeatures enabledFeatures{};

		auto indices = GetQueueFamilyIndices();
		std::vector<VkDeviceQueueCreateInfo> queues{};
		std::set<uint32_t> uniqueIndices = {
			*indices.graphicsFamily, *indices.presentFamily, *indices.computeFamily
		};

		float priority = 1.f;
		for (const uint32_t& family : uniqueIndices) {
			VkDeviceQueueCreateInfo queueInfo{};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = family;
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &priority;
			queues.push_back(queueInfo);
		}

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pEnabledFeatures = &enabledFeatures;
		if (enableValidation) { //Already checked in Instance
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()); //Checked in IsDeviceSuitable
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queues.size());
		createInfo.pQueueCreateInfos = queues.data();

		VULKAN_CHECK(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device),
			"Failed to create device!");

		vkGetDeviceQueue(device, *indices.graphicsFamily, 0, &graphicsQueue);
		vkGetDeviceQueue(device, *indices.presentFamily, 0, &presentQueue);
		vkGetDeviceQueue(device, *indices.computeFamily, 0, &computeQueue);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = *indices.graphicsFamily;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		VULKAN_CHECK(vkCreateCommandPool(device, &poolInfo, nullptr, &graphicsPool), "Failed to create graphics command pool!");

		poolInfo.queueFamilyIndex = *indices.computeFamily;
		VULKAN_CHECK(vkCreateCommandPool(device, &poolInfo, nullptr, &computePool), "Failed to create compute command pool!");
	}

	Device::~Device() {
		vkDestroyCommandPool(device, graphicsPool, nullptr);
		vkDestroyCommandPool(device, computePool, nullptr);

		vkDestroyDevice(device, nullptr);

		vkDestroySurfaceKHR(instance, surface, nullptr);
	}

	void Device::LogInfo() const {
		uint32_t count;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &count, nullptr);
		std::vector<VkExtensionProperties> extensions(count);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &count, extensions.data());

		std::stringstream msg;
		msg << extensions.size() << " supported device extension(s): ";
		for (const auto& extension : extensions) {
			msg << "\n\t" << extension.extensionName << " " << vk::VersionString(extension.specVersion);
		}

		LOGNFNG(util::Logger::Vulkan, msg.str());

		msg = std::stringstream{};
		msg << deviceExtensions.size() << " required device extension(s): ";
		for (const auto& extension : deviceExtensions) {
			msg << "\n\t" << extension;
		}

		LOGNFNG(util::Logger::Vulkan, msg.str());

		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(physicalDevice, &props);
		LOGNFNG(util::Logger::Vulkan, "Selected physical device $.", props.deviceName);
	}

	Device::QueueFamilyIndices Device::GetQueueFamilyIndices(VkPhysicalDevice device) const {
		uint32_t count;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, nullptr);
		std::vector<VkQueueFamilyProperties> queues(count);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, queues.data());

		QueueFamilyIndices indices{};
		for (uint32_t i = 0; i < queues.size(); i++) {
			const auto& family = queues[i];

			if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			if (family.queueFlags & VK_QUEUE_COMPUTE_BIT) {
				indices.computeFamily = i;
			}

			VkBool32 supported;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &supported);
			if (supported) {
				indices.presentFamily = i;
			}

			if (indices.IsComplete()) {
				return indices;
			}
		}

		return indices;
	}

	Device::SwapchainCapabilities Device::GetSwapchainCapabilities(VkPhysicalDevice device) const {
		SwapchainCapabilities caps{};
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &caps.capabilities);

		uint32_t count;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr);
		if (count > 0) {
			caps.formats.resize(count);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, caps.formats.data());
		}

		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, nullptr);
		caps.presentModes.resize(count); //Garunteed for at least VK_PRESENT_MODE_FIFO_KHR to be available
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, caps.presentModes.data());

		return caps;
	}

	//Returns UINT32_MAX if none were found
	uint32_t Device::FindMemoryType(const VkMemoryRequirements& memReqs, VkMemoryPropertyFlags memProps) const {
		VkPhysicalDeviceMemoryProperties deviceMemProps;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceMemProps);

		for (uint32_t i = 0; i < deviceMemProps.memoryTypeCount; i++) {
			if (((1 << i) & memReqs.memoryTypeBits) &&
				(deviceMemProps.memoryTypes[i].propertyFlags & memProps) == memProps) {
				return i;
			}
		}

		return UINT32_MAX;
	}

	VkResult Device::CreateBuffer(
		VkDeviceSize size,
		VkBufferUsageFlags usage,
		VkMemoryPropertyFlags memProps,
		QueueFamilyIndices::Type families,
		VkBuffer& buffer,
		VkDeviceMemory& memory
	) const {
		VkBufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.size = size;
		createInfo.usage = usage;
		const std::vector<uint32_t> uniqueIndices = UniqueQueueFamilies(GetQueueFamilyIndices(), families);
		if (uniqueIndices.size() == 1) {
			createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
		}
		else {
			createInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = static_cast<uint32_t>(uniqueIndices.size());
			createInfo.pQueueFamilyIndices = uniqueIndices.data();
		}

		VkResult result = vkCreateBuffer(device, &createInfo, nullptr, &buffer);
		if (result != VK_SUCCESS) {
			return result;
		}

		VkMemoryRequirements memReqs;
		vkGetBufferMemoryRequirements(device, buffer, &memReqs);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memReqs.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memReqs, memProps);

		result = vkAllocateMemory(device, &allocInfo, nullptr, &memory);
		if (result != VK_SUCCESS) {
			return result;
		}

		return vkBindBufferMemory(device, buffer, memory, 0);
	}

	VkResult Device::CreateImage(
		VkFormat format,
		VkExtent3D extent,
		VkImageType type,
		uint32_t levels, uint32_t layers,
		VkImageTiling tiling,
		VkImageLayout initialLayout,
		VkSampleCountFlagBits samples,
		VkImageUsageFlags usage,
		VkMemoryPropertyFlags memProps,
		QueueFamilyIndices::Type families,
		VkImage& image,
		VkDeviceMemory& memory
	) const {
		VkImageCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		createInfo.format = format;
		createInfo.extent = extent;
		createInfo.usage = usage;
		createInfo.tiling = tiling;
		createInfo.imageType = type;
		createInfo.mipLevels = levels;
		createInfo.arrayLayers = layers;
		createInfo.samples = samples;
		createInfo.initialLayout = initialLayout;
		createInfo.usage = usage;
		const std::vector<uint32_t> uniqueIndices = UniqueQueueFamilies(GetQueueFamilyIndices(), families);
		if (uniqueIndices.size() == 1) {
			createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
		}
		else {
			createInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = static_cast<uint32_t>(uniqueIndices.size());
			createInfo.pQueueFamilyIndices = uniqueIndices.data();
		}
		
		VkResult result = vkCreateImage(device, &createInfo, nullptr, &image);
		if (result != VK_SUCCESS) {
			return result;
		}

		VkMemoryRequirements memReqs;
		vkGetImageMemoryRequirements(device, image, &memReqs);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memReqs.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memReqs, memProps);

		result = vkAllocateMemory(device, &allocInfo, nullptr, &memory);
		if (result != VK_SUCCESS) {
			return result;
		}

		return vkBindImageMemory(device, image, memory, 0);
	}

	VkResult Device::CreateImageView(
		VkImage image,
		VkFormat format,
		VkImageViewType type,
		uint32_t levels, uint32_t layers,
		VkImageAspectFlags aspect,
		VkImageView& view
	) const {
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = image;
		createInfo.format = format;
		createInfo.viewType = type;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = aspect;
		createInfo.subresourceRange.layerCount = layers;
		createInfo.subresourceRange.levelCount = levels;

		return vkCreateImageView(device, &createInfo, nullptr, &view);
	}

	VkCommandBuffer Device::BeginSingleTime() const {
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = graphicsPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		VULKAN_CHECK(vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer),
			"Failed to allocate single-time command buffer!");

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		VULKAN_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo),
			"Failed to begin single-time command buffer!");

		return commandBuffer;
	}

	VkCommandBuffer Device::BeginSingleTimeCompute() const {
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = computePool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		VULKAN_CHECK(vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer),
			"Failed to allocate single-time compute command buffer!");

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		VULKAN_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo),
			"Failed to begin single-time compute command buffer!");

		return commandBuffer;
	}

	void Device::EndSingleTime(VkCommandBuffer commandBuffer, VkFence* fence) const {
		VULKAN_CHECK(vkEndCommandBuffer(commandBuffer),
			"Failed to end single-time command buffer!");

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		if (fence == VK_NULL_HANDLE) {
			VULKAN_CHECK(vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE),
				"Failed to submit single-time command buffer!");

			vkQueueWaitIdle(graphicsQueue);
		}
		else {
			VkFenceCreateInfo fenceInfo{};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			VULKAN_CHECK(vkCreateFence(device, &fenceInfo, nullptr, fence),
				"Failed to create single-time fence!");

			VULKAN_CHECK(vkQueueSubmit(graphicsQueue, 1, &submitInfo, *fence),
				"Failed to submit single-time command buffer!");
		}
	}

	void Device::EndSingleTimeCompute(VkCommandBuffer commandBuffer, VkFence* fence) const {
		VULKAN_CHECK(vkEndCommandBuffer(commandBuffer),
			"Failed to end single-time compute command buffer!");

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		if (fence == VK_NULL_HANDLE) {
			VULKAN_CHECK(vkQueueSubmit(computeQueue, 1, &submitInfo, VK_NULL_HANDLE),
				"Failed to submit single-time compute command buffer!");

			vkQueueWaitIdle(computeQueue);
		}
		else {
			VkFenceCreateInfo fenceInfo{};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			VULKAN_CHECK(vkCreateFence(device, &fenceInfo, nullptr, fence),
				"Failed to create single-time compute fence!");

			VULKAN_CHECK(vkQueueSubmit(computeQueue, 1, &submitInfo, *fence),
				"Failed to submit single-time compute command buffer!");
		}
	}

	void Device::CopyBuffer(
		VkBuffer src,
		VkBuffer dst,
		VkDeviceSize size,
		VkDeviceSize srcOffset,
		VkDeviceSize dstOffset
	) const {
		VkBufferCopy copy{};
		copy.size = size;
		copy.srcOffset = srcOffset;
		copy.dstOffset = dstOffset;

		VkCommandBuffer commandBuffer = BeginSingleTime();
		vkCmdCopyBuffer(commandBuffer, src, dst, 1, &copy);
		EndSingleTime(commandBuffer);
	}

	void Device::CopyBufferToImage(
		VkBuffer src,
		VkImage dst,
		VkExtent3D extent,
		VkImageAspectFlags aspect,
		uint32_t level
	) const {
		VkBufferImageCopy copy{};
		copy.imageOffset = { 0, 0, 0 };
		copy.imageExtent = extent;
		copy.imageSubresource.aspectMask = aspect;
		copy.imageSubresource.layerCount = 1;
		copy.imageSubresource.mipLevel = level;

		VkCommandBuffer commandBuffer = BeginSingleTime();
		vkCmdCopyBufferToImage(commandBuffer, src, dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);
		EndSingleTime(commandBuffer);
	}

	void Device::CopyImageToBuffer(
		VkImage src,
		VkBuffer dst,
		VkExtent3D extent,
		VkImageAspectFlags aspect,
		uint32_t level
	) const {
		VkBufferImageCopy copy{};
		copy.imageOffset = { 0, 0, 0 };
		copy.imageExtent = extent;
		copy.imageSubresource.aspectMask = aspect;
		copy.imageSubresource.layerCount = 1;
		copy.imageSubresource.mipLevel = level;

		VkCommandBuffer commandBuffer = BeginSingleTime();
		vkCmdCopyImageToBuffer(commandBuffer, src, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dst, 1, &copy);
		EndSingleTime(commandBuffer);
	}

	void Device::CopyImage(
		VkImage src,
		VkImage dst,
		VkExtent3D extent,
		VkImageAspectFlags aspect,
		uint32_t srcLevel,
		uint32_t dstLevel
	) const {
		VkImageCopy copy{};
		copy.extent = extent;
		copy.srcSubresource.aspectMask = aspect;
		copy.srcSubresource.layerCount = 1;
		copy.srcSubresource.mipLevel = srcLevel;
		copy.dstSubresource.aspectMask = aspect;
		copy.dstSubresource.layerCount = 1;
		copy.dstSubresource.mipLevel = dstLevel;

		VkCommandBuffer commandBuffer = BeginSingleTime();
		vkCmdCopyImage(commandBuffer, src, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);
		EndSingleTime(commandBuffer);
	}
}