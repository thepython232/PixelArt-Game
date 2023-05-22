#pragma once

#include "Platform\Device.h"

namespace gfx {

	class Texture {
	public:
		struct SamplerSettings {
			VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
			VkBorderColor borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

			VkFilter filter = VK_FILTER_LINEAR;
			VkBool32 unnormalizedCoords = VK_FALSE;

			std::optional<VkCompareOp> compareOp = std::nullopt;

			VkBool32 enableAnisotropy = VK_TRUE;
			std::optional<float> maxAnisotropy = std::nullopt; //Opt means use maximum

			float minLod = 0.f;
			std::optional<float> maxLod = std::nullopt;
			float mipLodBias = 0.f;
			VkSamplerMipmapMode mipMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		};

		Texture(
			VkFormat format,
			VkExtent3D extent,
			VkImageUsageFlags usage,
			uint32_t mipLevels, uint32_t arrayLayers,
			VkSampleCountFlagBits samples,
			b8 createSampler,
			VkMemoryPropertyFlags memProps,
			uint32_t queueFamilies,
			VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL,
			VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			const SamplerSettings& samplerSettings = {}
		);

		~Texture();

		//Copyable and Movable
		Texture(const Texture& other);
		Texture& operator=(const Texture& other);
		Texture(Texture&& other) noexcept;
		Texture& operator=(Texture&& other) noexcept;

		//Getters and setters
		inline operator VkImage() const { return image; }
		inline VkImage Image() const { return image; }
		inline VkImageView ImageView() const { return imageView; }
		inline VkSampler Sampler() const { return sampler; }
		inline VkFormat Format() const { return format; }
		inline VkImageTiling Tiling() const { return tiling; }
		inline VkExtent3D Extent() const { return extent; }
		inline uint32_t Width() const { return extent.width; }
		inline uint32_t Height() const { return extent.height; }
		inline uint32_t Depth() const { return extent.depth; }
		inline VkImageLayout CurrentLayout() const { return layout; }
		inline uint32_t MipLevels() const { return levels; }
		inline uint32_t ArrayLayers() const { return layers; }
		inline VkSampleCountFlagBits NumSamples() const { return samples; }

		inline VkImageType ImageType() const {
			return (extent.depth == 1) ? VK_IMAGE_TYPE_2D
				: ((extent.height == 1) ? VK_IMAGE_TYPE_1D : VK_IMAGE_TYPE_3D);
		}

		inline VkImageViewType ImageViewType() const {
			uint32_t type = (extent.depth == 1) ? VK_IMAGE_VIEW_TYPE_2D
				: ((extent.height == 1) ? VK_IMAGE_VIEW_TYPE_1D : VK_IMAGE_VIEW_TYPE_3D);
			if (layers > 1) {
				type += 4;
			}
			return (VkImageViewType)type;
		}

		inline VkImageAspectFlags ImageAspect() const {
			VkImageAspectFlags aspect = 0;
			if (vk::IsStencilFormat(format)) {
				aspect |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
			if (vk::IsDepthFormat(format)) {
				aspect |= VK_IMAGE_ASPECT_DEPTH_BIT;
			}
			if (vk::IsColorFormat(format)) {
				aspect |= VK_IMAGE_ASPECT_COLOR_BIT;
			}
			return aspect;
		}

		static std::unique_ptr<Texture> Load(
			const std::string& file,
			VkImageUsageFlags usage,
			VkMemoryPropertyFlags memProps,
			uint32_t families,
			b8 mipmap = true,
			std::optional<SamplerSettings> samplerSettings = SamplerSettings{}
		);

		void TransitionLayout(VkImageLayout newLayout);
		void FillMipmaps(VkImageLayout newLayout);

		VkDescriptorImageInfo DescriptorInfo() const;

	private:
		VkImage image;
		VkDeviceMemory memory;
		VkImageView imageView;
		VkSampler sampler = VK_NULL_HANDLE;

		VkFormat format;
		VkExtent3D extent;
		VkImageLayout layout;
		uint32_t levels, layers;
		VkSampleCountFlagBits samples;
		VkImageTiling tiling;

		VkImageUsageFlags usage;
		VkMemoryPropertyFlags memProps;
		uint32_t queueFamilies;
		std::optional<SamplerSettings> samplerSettings;
	};
}