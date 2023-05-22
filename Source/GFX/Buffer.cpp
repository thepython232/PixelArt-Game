#include "Buffer.h"
#include "Platform/Platform.h"
#include "State.h"

static VkDeviceSize AlignInstanceSize(VkDeviceSize instanceSize, VkDeviceSize align) {
	return (instanceSize + align) & (~align);
}

namespace gfx {

	Buffer::Buffer(
		VkDeviceSize instanceSize,
		VkDeviceSize instanceCount,
		VkBufferUsageFlags usage,
		VkMemoryPropertyFlags memProps,
		uint32_t queueFamilies,
		VkDeviceSize align
	) : usage(usage), memProps(memProps), families(queueFamilies) {
		this->instanceSize = AlignInstanceSize(instanceSize, align);
		bufferSize = this->instanceSize * instanceCount;

		VULKAN_CHECK(
			global.platform->device->CreateBuffer(
				bufferSize,
				usage | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, //TODO: get rid of this
				memProps,
				families,
				buffer,
				memory
			), "Failed to create buffer!"
		);
	}

	Buffer::Buffer(const Buffer& other)
		: instanceSize(other.instanceSize),
		bufferSize(other.bufferSize),
		usage(other.usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT),
		memProps(other.memProps),
		families(other.families) {

		VULKAN_CHECK(
			global.platform->device->CreateBuffer(
				bufferSize,
				usage,
				memProps,
				families,
				buffer,
				memory
			), "Failed to create buffer!"
		);

		global.platform->device->CopyBuffer(other, buffer, bufferSize);
	}

	Buffer& Buffer::operator=(const Buffer& other) {
		if (buffer) {
			vkFreeMemory(*global.platform->device, memory, nullptr);
			vkDestroyBuffer(*global.platform->device, buffer, nullptr);
		}

		instanceSize = other.instanceSize;
		bufferSize = other.bufferSize;
		usage = other.usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		memProps = other.memProps;
		families = other.families;

		VULKAN_CHECK(
			global.platform->device->CreateBuffer(
				bufferSize,
				usage,
				memProps,
				families,
				buffer,
				memory
			), "Failed to create buffer!"
		);

		global.platform->device->CopyBuffer(other, buffer, bufferSize);

		return *this;
	}

	Buffer::Buffer(Buffer&& other) noexcept
		: buffer(other.buffer),
		memory(other.memory),
		bufferSize(other.bufferSize),
		instanceSize(other.instanceSize),
		usage(other.usage),
		memProps(other.memProps),
		families(other.families) {
		other.buffer = VK_NULL_HANDLE;
		other.memory = VK_NULL_HANDLE;
	}

	Buffer& Buffer::operator=(Buffer&& other) noexcept {
		if (buffer) {
			vkFreeMemory(*global.platform->device, memory, nullptr);
			vkDestroyBuffer(*global.platform->device, buffer, nullptr);
		}

		buffer = other.buffer;
		memory = other.memory;
		bufferSize = other.bufferSize;
		instanceSize = other.instanceSize;
		usage = other.usage;
		memProps = other.memProps;
		families = other.families;

		other.buffer = VK_NULL_HANDLE;
		other.memory = VK_NULL_HANDLE;

		return *this;
	}

	Buffer::~Buffer() {
		if (buffer) {
			vkFreeMemory(*global.platform->device, memory, nullptr);
			vkDestroyBuffer(*global.platform->device, buffer, nullptr);
		}
	}

	VkDescriptorBufferInfo Buffer::DescriptorInfo() const {
		VkDescriptorBufferInfo info{};
		info.buffer = buffer;
		info.offset = 0;
		info.range = bufferSize;
		return info;
	}

	void* Buffer::Map(VkDeviceSize size, VkDeviceSize offset) {
		VULKAN_CHECK(vkMapMemory(*global.platform->device, memory, offset, size, 0, &mapped),
			"Failed to map buffer memory!");
		isMapped = true;
		return mapped;
	}

	void Buffer::UnMap() {
		vkUnmapMemory(*global.platform->device, memory);
	}

	void Buffer::Write(const void* data, VkDeviceSize size, VkDeviceSize offset) {
		ASSERT(isMapped, "Cannot write no non-mapped buffer!");

		usize realSize = (size == VK_WHOLE_SIZE) ? bufferSize : size;

		ASSERT(realSize + offset <= bufferSize, "Buffer write too large!");
		
		u8* dst = reinterpret_cast<u8*>(mapped);
		dst += offset;

		std::memcpy((void*)dst, data, realSize);
	}

	void Buffer::WriteToIndex(const void* data, uint32_t index) {
		Write(data, instanceSize, index * instanceSize);
	}

	VkResult Buffer::Flush(VkDeviceSize size, VkDeviceSize offset) {
		VkMappedMemoryRange range{};
		range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		range.memory = memory;
		range.offset = offset;
		range.size = size;

		return vkFlushMappedMemoryRanges(*global.platform->device, 1, &range);
	}

	VkResult Buffer::FlushIndex(uint32_t index) {
		return Flush(instanceSize, instanceSize * index);
	}

	VkResult Buffer::Invalidate(VkDeviceSize size, VkDeviceSize offset) {
		VkMappedMemoryRange range{};
		range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		range.memory = memory;
		range.offset = offset;
		range.size = size;

		return vkInvalidateMappedMemoryRanges(*global.platform->device, 1, &range);
	}

	VkResult Buffer::InvalidateIndex(uint32_t index) {
		return Invalidate(instanceSize, instanceSize * index);
	}
}