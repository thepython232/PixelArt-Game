#pragma once

#include "Platform/Device.h"

namespace gfx {

	class Buffer {
	public:
		Buffer(
			VkDeviceSize instanceSize,
			VkDeviceSize instanceCount,
			VkBufferUsageFlags usage,
			VkMemoryPropertyFlags memProps,
			uint32_t queueFamilies,
			VkDeviceSize align = 0u
		);

		Buffer(const Buffer& other);
		Buffer& operator=(const Buffer& other);
		Buffer(Buffer&& other) noexcept;
		Buffer& operator=(Buffer&& other) noexcept;

		~Buffer();

		inline operator VkBuffer() const { return buffer; }
		inline VkBuffer GetBuffer() const { return buffer; }
		inline VkDeviceSize WholeSize() const { return bufferSize; }
		inline VkDeviceSize InstanceSize() const { return instanceSize; }
		inline VkDeviceSize InstanceCount() const { return bufferSize / instanceSize; }
		inline b8 IsMapped() const { return isMapped; }
		inline void* GetMappedMemory() const { return mapped; }

		VkDescriptorBufferInfo DescriptorInfo() const;

		void* Map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0u);
		void UnMap();

		void Write(const void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0u);
		void WriteToIndex(const void* data, uint32_t index);

		VkResult Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0u);
		VkResult FlushIndex(uint32_t index);

		VkResult Invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0u);
		VkResult InvalidateIndex(uint32_t index);

	private:
		VkBuffer buffer;
		VkDeviceMemory memory;
		VkDeviceSize instanceSize, bufferSize;

		void* mapped;
		b8 isMapped = false;

		VkBufferUsageFlags usage;
		VkMemoryPropertyFlags memProps;
		uint32_t families;
	};
}