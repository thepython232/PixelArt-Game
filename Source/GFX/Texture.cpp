#include "Texture.h"
#include "Platform\Platform.h"
#include "State.h"
#include "Util\File.h"
#define STB_IMAGE_IMPLEMENTATION
//#include "Util\STBImage.h"
#include "stb_image.h"
#include "Buffer.h"

static void GetTransitionInfo(VkImageLayout layout, VkAccessFlags& access, VkPipelineStageFlags& stages) {
	if (layout == VK_IMAGE_LAYOUT_UNDEFINED) {
		access = 0;
		stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	}
	else if (layout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
		access = VK_ACCESS_TRANSFER_READ_BIT;
		stages = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		access = VK_ACCESS_TRANSFER_WRITE_BIT;
		stages = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		access = VK_ACCESS_SHADER_READ_BIT;
		stages = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
	}
	else {
		WARN("Unsupported layout transition ($)!", layout);
	}
}

namespace gfx {

	Texture::Texture(
		VkFormat format,
		VkExtent3D extent,
		VkImageUsageFlags usage,
		uint32_t mipLevels, uint32_t arrayLayers,
		VkSampleCountFlagBits samples,
		b8 createSampler,
		VkMemoryPropertyFlags memProps,
		uint32_t queueFamilies,
		VkImageTiling tiling,
		VkImageLayout initialLayout,
		const SamplerSettings& samplerSettings
	)
		: format(format),
		extent(extent),
		usage(usage | VK_IMAGE_USAGE_TRANSFER_SRC_BIT
			| (createSampler ? VK_IMAGE_USAGE_SAMPLED_BIT : 0)),
		levels(mipLevels),
		layers(arrayLayers),
		samples(samples),
		tiling(tiling),
		layout(initialLayout),
		memProps(memProps),
		queueFamilies(queueFamilies),
		samplerSettings(createSampler ? std::make_optional(samplerSettings) : std::nullopt)
	{
		VULKAN_CHECK(
			global.platform->device->CreateImage(
				format,
				extent,
				ImageType(),
				levels, layers,
				tiling,
				initialLayout,
				samples,
				this->usage,
				memProps,
				queueFamilies,
				image, memory
			), "Failed to create texture image!"
		);

		VULKAN_CHECK(
			global.platform->device->CreateImageView(
				image,
				format,
				ImageViewType(),
				levels, layers,
				ImageAspect(),
				imageView
			), "Failed to create texture image view!"
		);

		if (createSampler) {
			VkSamplerCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

			createInfo.addressModeU = samplerSettings.addressMode;
			createInfo.addressModeV = samplerSettings.addressMode;
			createInfo.addressModeW = samplerSettings.addressMode;
			createInfo.borderColor = samplerSettings.borderColor;

			createInfo.anisotropyEnable = samplerSettings.enableAnisotropy == VK_TRUE
				&& global.platform->device->Features().samplerAnisotropy == VK_TRUE;
			createInfo.maxAnisotropy = samplerSettings.maxAnisotropy.value_or(
				global.platform->device->Limits().maxSamplerAnisotropy);

			createInfo.minFilter = samplerSettings.filter;
			createInfo.magFilter = samplerSettings.filter;
			createInfo.unnormalizedCoordinates = samplerSettings.unnormalizedCoords;

			createInfo.compareEnable = samplerSettings.compareOp.has_value();
			createInfo.compareOp = samplerSettings.compareOp.value_or(VK_COMPARE_OP_NEVER);

			createInfo.mipmapMode = samplerSettings.mipMode;
			createInfo.minLod = samplerSettings.minLod;
			createInfo.maxLod = samplerSettings.maxLod.value_or(
				static_cast<float>(levels));
			createInfo.mipLodBias = samplerSettings.mipLodBias;

			VULKAN_CHECK(vkCreateSampler(
				*global.platform->device, &createInfo, nullptr, &sampler),
				"Failed to create texture sampler!");
		}
	}

	Texture::Texture(const Texture& other) {
		image = VK_NULL_HANDLE;
		*this = other;
	}

	Texture& Texture::operator=(const Texture& other) {
		format = other.format;
		extent = other.extent;
		layout = other.layout;
		levels = other.levels;
		layers = other.layers;
		samples = other.samples;
		tiling = other.tiling;
		usage = other.usage | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		memProps = other.memProps;
		queueFamilies = other.queueFamilies;
		samplerSettings = other.samplerSettings;

		if (image) {
			vkDestroyImageView(*global.platform->device, imageView, nullptr);
			vkFreeMemory(*global.platform->device, memory, nullptr);
			vkDestroyImage(*global.platform->device, image, nullptr);

			if (sampler) {
				vkDestroySampler(*global.platform->device, sampler, nullptr);
			}
		}

		VULKAN_CHECK(
			global.platform->device->CreateImage(
				format,
				extent,
				ImageType(),
				levels, layers,
				tiling,
				VK_IMAGE_LAYOUT_UNDEFINED,
				samples,
				this->usage,
				memProps,
				queueFamilies,
				image, memory
			), "Failed to create texture image!"
		);

		VULKAN_CHECK(
			global.platform->device->CreateImageView(
				image,
				format,
				ImageViewType(),
				levels, layers,
				ImageAspect(),
				imageView
			), "Failed to create texture image view!"
		);

		if (samplerSettings) {
			VkSamplerCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

			createInfo.addressModeU = samplerSettings->addressMode;
			createInfo.addressModeV = samplerSettings->addressMode;
			createInfo.addressModeW = samplerSettings->addressMode;
			createInfo.borderColor = samplerSettings->borderColor;

			createInfo.anisotropyEnable = samplerSettings->enableAnisotropy;
			createInfo.maxAnisotropy = samplerSettings->maxAnisotropy.value_or(
				global.platform->device->Limits().maxSamplerAnisotropy);

			createInfo.minFilter = samplerSettings->filter;
			createInfo.magFilter = samplerSettings->filter;
			createInfo.unnormalizedCoordinates = samplerSettings->unnormalizedCoords;

			createInfo.compareEnable = samplerSettings->compareOp.has_value();
			createInfo.compareOp = samplerSettings->compareOp.value_or(VK_COMPARE_OP_NEVER);

			createInfo.mipmapMode = samplerSettings->mipMode;
			createInfo.minLod = samplerSettings->minLod;
			createInfo.maxLod = samplerSettings->maxLod.value_or(
				static_cast<float>(levels));
			createInfo.mipLodBias = samplerSettings->mipLodBias;

			VULKAN_CHECK(vkCreateSampler(
				*global.platform->device, &createInfo, nullptr, &sampler),
				"Failed to create texture sampler!");
		}

		auto commandBuffer =
			global.platform->device->BeginSingleTime();

		//Transition both images
		VkImageMemoryBarrier barrier{};
		barrier.image = image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = ImageAspect();
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = layers;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = levels;
		barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		vkCmdPipelineBarrier(
			commandBuffer,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		VkAccessFlags access;
		VkPipelineStageFlags stage;
		GetTransitionInfo(other.layout, access, stage);

		barrier.image = other.image;
		barrier.oldLayout = other.layout;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = access;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(
			commandBuffer,
			stage, VK_PIPELINE_STAGE_TRANSFER_BIT,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		//Copy the images
		for (uint32_t i = 0; i < levels; i++) {
			VkImageCopy copy{};
			copy.extent = extent;

			copy.srcOffset = { 0, 0, 0 };
			copy.srcSubresource.aspectMask = ImageAspect();
			copy.srcSubresource.baseArrayLayer = 0;
			copy.srcSubresource.layerCount = layers;
			copy.srcSubresource.mipLevel = i;

			copy.dstOffset = { 0, 0, 0 };
			copy.dstSubresource.aspectMask = ImageAspect();
			copy.dstSubresource.baseArrayLayer = 0;
			copy.dstSubresource.layerCount = layers;
			copy.dstSubresource.mipLevel = i;

			vkCmdCopyImage(
				commandBuffer,
				other.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &copy
			);
		}

		//Transition them back
		VkImageMemoryBarrier srcBarrier = barrier;
		barrier.image = image;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = other.layout;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = access;

		VkImageMemoryBarrier dstBarrier = srcBarrier;
		barrier.image = other.image;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		VkImageMemoryBarrier barriers[] = { srcBarrier, dstBarrier };

		vkCmdPipelineBarrier(
			commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, stage,
			0,
			0, nullptr,
			0, nullptr,
			2, barriers
		);

		global.platform->device->EndSingleTime(commandBuffer);

		return *this;
	}

	Texture::Texture(Texture&& other) noexcept {
		image = VK_NULL_HANDLE;
		*this = std::move(other);
	}

	Texture& Texture::operator=(Texture&& other) noexcept {
		if (image) {
			vkDestroyImageView(*global.platform->device, imageView, nullptr);
			vkFreeMemory(*global.platform->device, memory, nullptr);
			vkDestroyImage(*global.platform->device, image, nullptr);

			if (sampler) {
				vkDestroySampler(*global.platform->device, sampler, nullptr);
			}
		}

		image = other.image;
		memory = other.memory;
		imageView = other.imageView;
		sampler = other.sampler;

		format = other.format;
		extent = other.extent;
		layout = other.layout;
		levels = other.levels;
		layers = other.layers;
		samples = other.samples;
		tiling = other.tiling;
		usage = other.usage;
		memProps = other.memProps;
		queueFamilies = other.queueFamilies;
		samplerSettings = other.samplerSettings;

		other.image = VK_NULL_HANDLE;

		return *this;
	}

	Texture::~Texture() {
		if (image) {
			vkDestroyImageView(*global.platform->device, imageView, nullptr);
			vkFreeMemory(*global.platform->device, memory, nullptr);
			vkDestroyImage(*global.platform->device, image, nullptr);

			if (sampler != VK_NULL_HANDLE) {
				vkDestroySampler(*global.platform->device, sampler, nullptr);
			}
		}
	}

	std::unique_ptr<Texture> Texture::Load(
		const std::string& file,
		VkImageUsageFlags usage,
		VkMemoryPropertyFlags memProps,
		uint32_t families,
		b8 mipmap,
		std::optional<SamplerSettings> samplerSettings
	) {
		ASSERT(util::FileExists(file), "Image file " + file + " does not exist!");

		const std::vector<u8> imageData = util::ReadFileBinary(file).Unwrap();

		int width, height, comp;
		stbi_uc* pixels = stbi_load_from_memory(
			imageData.data(), static_cast<int>(imageData.size()),
			&width, &height, &comp, STBI_rgb_alpha);

		if (!pixels) {
			ERROR(-1, util::Logger::GFX, "Failed to load texture: $!", stbi_failure_reason());
		}

		auto pixelBuffer = std::make_unique<Buffer>(
			sizeof(stbi_uc),
			width * height * 4,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			vk::QueueFamilies::Graphics
			);
		pixelBuffer->Map();
		pixelBuffer->Write((void*)pixels);
		pixelBuffer->UnMap();

		stbi_image_free(pixels);

		uint32_t levels = 1;
		if (mipmap) {
			levels = static_cast<uint32_t>(math::Floor(math::Log2(
				static_cast<f32>(math::Min(width, height))))) + 1;
		}

		VkExtent3D extent{ static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 };

		auto texture = std::make_unique<Texture>(
			VK_FORMAT_R8G8B8A8_SRGB,
			extent,
			VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | usage,
			levels, 1u,
			VK_SAMPLE_COUNT_1_BIT,
			samplerSettings.has_value(),
			memProps,
			families,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_LAYOUT_UNDEFINED,
			samplerSettings.value_or(SamplerSettings{})
		);

		texture->TransitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		global.platform->device->CopyBufferToImage(*pixelBuffer, *texture, extent, texture->ImageAspect());

		if (mipmap) {
			texture->FillMipmaps(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		}
		else {
			texture->TransitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		}

		return texture;
	}

	void Texture::TransitionLayout(VkImageLayout newLayout) {
		auto commandBuffer =
			global.platform->device->BeginSingleTime();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = ImageAspect();
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = layers;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = levels;

		barrier.oldLayout = layout;
		barrier.newLayout = newLayout;
		
		VkAccessFlags srcAccess, dstAccess;
		VkPipelineStageFlags srcStages, dstStages;
		GetTransitionInfo(layout, srcAccess, srcStages);
		GetTransitionInfo(newLayout, dstAccess, dstStages);

		barrier.srcAccessMask = srcAccess;
		barrier.dstAccessMask = dstAccess;

		vkCmdPipelineBarrier(
			commandBuffer,
			srcStages, dstStages,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		global.platform->device->EndSingleTime(commandBuffer);
	}

	void Texture::FillMipmaps(VkImageLayout dstLayout) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(global.platform->device->PhysicalDevice(), format, &props);

		ASSERT(props.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT, "Format doesn't support linear blitting!");

		auto commandBuffer = global.platform->device->BeginSingleTime();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = ImageAspect();
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = layers;
		barrier.subresourceRange.levelCount = 1;

		int32_t mipWidth = extent.width;
		int32_t mipHeight = extent.height;
		int32_t mipDepth = extent.depth;

		VkAccessFlags access;
		VkPipelineStageFlags stages;
		GetTransitionInfo(dstLayout, access, stages);

		for (uint32_t i = 1; i < levels; i++) {
			//Transition the previous level to optimal as src for transfer
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.subresourceRange.baseMipLevel = i - 1;

			vkCmdPipelineBarrier(
				commandBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
				0,
				0, nullptr,
				0, nullptr,
				1, &barrier
			);

			//Blit from level i - 1 to i
			VkImageBlit blit{};
			blit.srcOffsets[0] = { 0, 0, 0 };
			blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
			blit.srcSubresource.aspectMask = ImageAspect();
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount = layers;
			blit.srcSubresource.mipLevel = i - 1;
			blit.dstOffsets[0] = { 0, 0, 0 };
			blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1,
				mipHeight > 1 ? mipHeight / 2 : 1,
				mipDepth > 1 ? mipDepth : 1 };
			blit.dstSubresource.aspectMask = ImageAspect();
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount = layers;
			blit.dstSubresource.mipLevel = i;

			vkCmdBlitImage(
				commandBuffer,
				image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &blit,
				VK_FILTER_LINEAR
			);

			//Transition the previous level to dstLayout
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = access;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = dstLayout;
			barrier.subresourceRange.baseMipLevel = i - 1;

			vkCmdPipelineBarrier(
				commandBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, stages,
				0,
				0, nullptr,
				0, nullptr,
				1, &barrier
			);

			if (mipWidth > 1) mipWidth /= 2;
			if (mipHeight > 1) mipHeight /= 2;
			if (mipDepth > 1) mipDepth /= 2;
		}

		//Transition the final layout to dstLayout (never blitted from)
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = access;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = dstLayout;
		barrier.subresourceRange.baseMipLevel = levels - 1;

		vkCmdPipelineBarrier(
			commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, stages,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		global.platform->device->EndSingleTime(commandBuffer);

		layout = dstLayout;
	}

	VkDescriptorImageInfo Texture::DescriptorInfo() const {
		VkDescriptorImageInfo info{};
		info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		info.imageView = imageView;
		info.sampler = sampler;
		return info;
	}
}