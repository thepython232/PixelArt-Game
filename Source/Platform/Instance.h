#pragma once

#include "Util\Types.h"
#include "Util\Std.h"
#include "Util\Vulkan.h"
#include "Util\Log.h"

namespace platform {

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

#ifdef GAME_IS_DEBUG
	constexpr bool enableValidation = true;
#else
	constexpr bool enableValidation = false;
#endif

	class Instance {
	public:
		Instance();
		~Instance();

		Instance(const Instance& other) = delete;
		Instance& operator=(const Instance& other) = delete;
		Instance(Instance&& other) noexcept = delete;
		Instance& operator=(Instance&& other) noexcept = delete;

		inline operator VkInstance() const { return instance; }
		inline VkInstance GetInstance() const { return instance; }

		inline b8 IsInitialized() const { return initialized; }

		void LogInfo() const;
		
	private:
		VkInstance instance;
		VkDebugUtilsMessengerEXT messenger;

		b8 initialized = false;
	};
}