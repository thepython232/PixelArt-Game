#pragma once

#include "Util\Types.h"
#include "Util\Std.h"
#include "Util\Vulkan.h"

namespace gfx {

	class RenderTarget {
	public:
		enum Type : u32 {
			Color,
			Resolve,
			Input,
			Depth
		};

		RenderTarget(
			VkFormat format,
			VkExtent2D extent,
			VkSampleCountFlagBits samples,
			b8 sampled = false
		);

		~RenderTarget();

		void Resize(VkExtent2D extent);

		inline usize UID() const { return uid; }
		inline u32 NumViews() const { return static_cast<u32>(views.size()); }
		inline VkFormat GetFormat() const { return format; }
		inline VkSampleCountFlagBits NumSamples() const { return samples; }
		inline VkImageView GetView(uint32_t index) const { return views[index]; }
		VkDescriptorImageInfo GetDescriptorInfo(uint32_t index) const;

	private:
		usize uid;

		std::vector<VkImage> images;
		std::vector<VkImageView> views;
		std::vector<VkDeviceMemory> memory;
		VkFormat format;
		VkSampleCountFlagBits samples;

		static usize nextID;
	};
}