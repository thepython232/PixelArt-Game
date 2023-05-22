#pragma once

//Platform-specific files
#include "Types.h"
#include "Math.h"
#include <vector>
#include <string>
#ifdef GAME_IS_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(GAME_IS_LINUX)
#ifdef GAME_USE_WAYLAND
#define VK_USE_PLATFORM_WAYLAND_KHR
#else
#include VK_USE_PLATFORM_XCB_KHR
#endif
#elif defined(GAME_IS_OSX)
#include VK_USE_PLATFORM_MACOS_MVK
#elif defined(GAME_IS_IOS)
#include VK_USE_PLATFORM_IOS_MVK
#elif defined(GAME_IS_ANDRIOD)
#include VK_USE_PLATFORM_ANDROID_KHR
#endif

#include "vulkan\vulkan.h"
#ifdef GAME_IS_WINDOWS
#undef VK_USE_PLATFORM_WIN32_KHR
#include "vulkan\vk_enum_string_helper.h"
#define VK_USE_PLATFORM_WIN32_KHR
#else
#include "vulkan\vk_enum_string_helper.h"
#endif

#define __DECL_VULKAN_STRING_FUNCTION(_T) \
inline std::string ToString(_T val) { \
	return std::string(string_##_T(val)); \
}

namespace vk {

	constexpr u32 MAX_FRAMES_IN_FLIGHT = 2;

	struct QueueFamilies {
		enum {
			Graphics = 1 << 0,
			Present = 1 << 1,
			Compute = 1 << 2
		};
	};

	//Helper functions
	inline bool IsDepthFormat(VkFormat format) {
		return format == VK_FORMAT_D16_UNORM ||
			format == VK_FORMAT_D16_UNORM_S8_UINT ||
			format == VK_FORMAT_D24_UNORM_S8_UINT ||
			format == VK_FORMAT_D32_SFLOAT ||
			format == VK_FORMAT_D32_SFLOAT_S8_UINT;
	}

	inline bool IsStencilFormat(VkFormat format) {
		return format == VK_FORMAT_D16_UNORM_S8_UINT ||
			format == VK_FORMAT_D24_UNORM_S8_UINT ||
			format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
			format == VK_FORMAT_S8_UINT;
	}

	inline bool IsColorFormat(VkFormat format) {
		return !IsDepthFormat(format) && !IsStencilFormat(format); //TODO: actually check it
	}

	//Returns VK_FORMAT_UNDEFINED if none are supported
	inline VkFormat FindSupportedFormat(VkPhysicalDevice device, const std::vector<VkFormat> candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
		for (const auto& format : candidates) {
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(device, format, &props);
			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}

		return VK_FORMAT_UNDEFINED;
	}

	inline std::string VersionString(uint32_t version) {
		return std::string(
			std::to_string(VK_API_VERSION_MAJOR(version)) + "."
			+ std::to_string(VK_API_VERSION_MINOR(version)) + "."
			+ std::to_string(VK_API_VERSION_PATCH(version))
		);
	}

	inline VkImageAspectFlags AspectFromFormat(VkFormat format) {
		VkImageAspectFlags aspect = 0;
		if (IsDepthFormat(format)) {
			aspect |= VK_IMAGE_ASPECT_DEPTH_BIT;
		}
		if (IsStencilFormat(format)) {
			aspect |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
		if (IsColorFormat(format)) {
			aspect |= VK_IMAGE_ASPECT_COLOR_BIT;
		}
		return aspect;
	}

	inline VkExtent2D VecToExtent(const ivec2& v) {
		return VkExtent2D{
			static_cast<uint32_t>(v.x),
			static_cast<uint32_t>(v.y) };
	}

	inline VkExtent2D VecToExtent(const uvec2& v) {
		return VkExtent2D{ v.x, v.y };
	}

	inline VkExtent3D VecToExtent(const ivec3& v) {
		return VkExtent3D{
			static_cast<uint32_t>(v.x),
			static_cast<uint32_t>(v.y),
			static_cast<uint32_t>(v.z) };
	}

	inline VkExtent3D VecToExtent(const uvec3& v) {
		return VkExtent3D{ v.x, v.y, v.z };
	}

	inline uvec2 ExtentToVec(VkExtent2D extent) {
		return uvec2{ extent.width, extent.height };
	}

	inline uvec3 ExtentToVec(VkExtent3D extent) {
		return uvec3{ extent.width, extent.height, extent.depth };
	}

	inline VkExtent3D Extent2DTo3D(const VkExtent2D& extent) {
		return VkExtent3D{ extent.width, extent.height, 1 };
	}
	
	__DECL_VULKAN_STRING_FUNCTION(VkResult)
	__DECL_VULKAN_STRING_FUNCTION(VkFormat)
	__DECL_VULKAN_STRING_FUNCTION(VkPresentModeKHR)
	__DECL_VULKAN_STRING_FUNCTION(VkColorSpaceKHR)
	__DECL_VULKAN_STRING_FUNCTION(VkImageLayout)
}

constexpr inline bool operator==(const VkSurfaceFormatKHR& a, const VkSurfaceFormatKHR& b) {
	return a.format == b.format && a.colorSpace == b.colorSpace;
}

#define VULKAN_CHECK(_f, _m) do { \
	VkResult _res = _f; \
	if (_res != VK_SUCCESS) { \
		ERROR((i32)_res, util::Logger::Vulkan, "Vulkan assertion failed ($): $", vk::ToString(_res), _m); \
	} \
} while(0)