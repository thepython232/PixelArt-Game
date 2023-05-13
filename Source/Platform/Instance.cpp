#include "Instance.h"
#include "State.h"
#include "Util\Log.h"
#include "GLFW\glfw3.h"

static VKAPI_ATTR VkBool32 VKAPI_CALL MessageCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageTypes,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData
) {
	platform::Instance* instance = reinterpret_cast<platform::Instance*>(pUserData);

	if (instance->IsInitialized()) {
		util::Logger::Severity severity = util::Logger::Debug;
		if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			severity = util::Logger::Warn;
		else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			severity = util::Logger::Error;

		global.log->LogNoFile("Validation Message: $.", util::Logger::Vulkan, severity, pCallbackData->pMessage);
	}

	return VK_FALSE;
}

static std::vector<const char*> GetRequiredInstanceExtensions() {
	uint32_t count;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&count);
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + count);

	if (platform::enableValidation) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

static bool CheckExtensionSupport(const std::vector<const char*>& extensions) {
	uint32_t count;
	vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
	std::vector<VkExtensionProperties> props(count);
	vkEnumerateInstanceExtensionProperties(nullptr, &count, props.data());

	for (const auto& extension : extensions) {
		bool found = false;
		for (const auto& prop : props) {
			if (strcmp(extension, prop.extensionName) == 0) {
				found = true;
				break;
			}
		}

		if (!found) {
			return false;
		}
	}

	return true;
}

static bool CheckLayerSupport(const std::vector<const char*>& layers) {
	uint32_t count;
	vkEnumerateInstanceLayerProperties(&count, nullptr);
	std::vector<VkLayerProperties> props(count);
	vkEnumerateInstanceLayerProperties(&count, props.data());

	for (const auto& layer : layers) {
		bool found = false;
		for (const auto& prop : props) {
			if (strcmp(layer, prop.layerName) == 0) {
				found = true;
				break;
			}
		}

		if (!found) {
			return false;
		}
	}

	return true;
}

static VkResult CreateDebugMessengerEXT(
	VkInstance instance,
	const VkDebugUtilsMessengerCreateInfoEXT* createInfo,
	const VkAllocationCallbacks* allocator,
	VkDebugUtilsMessengerEXT* messenger
) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func) {
		return func(instance, createInfo, allocator, messenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

static void DestroyDebugMessengerEXT(
	VkInstance instance,
	VkDebugUtilsMessengerEXT messenger,
	const VkAllocationCallbacks* allocator
) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func) {
		func(instance, messenger, allocator);
	}
}

namespace platform {

	Instance::Instance() {
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.apiVersion = VK_API_VERSION_1_0;
		appInfo.pApplicationName = "GAME";
		appInfo.applicationVersion = VK_API_VERSION_1_0;
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		const std::vector<const char*> extensions = GetRequiredInstanceExtensions();
		ASSERT(CheckExtensionSupport(extensions), "Missing instance extension(s)!");
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT messengerInfo{};
		messengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		messengerInfo.pUserData = static_cast<void*>(this);
		messengerInfo.pfnUserCallback = &MessageCallback;
		messengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
		messengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		if (enableValidation) {
			if (!CheckLayerSupport(validationLayers)) {
				WARN("Validation layers requested but unavailable!");
			}

			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
			createInfo.pNext = static_cast<const void*>(&messengerInfo);
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		VULKAN_CHECK(vkCreateInstance(&createInfo, nullptr, &instance), "Failed to create instance!");

		initialized = true;

		VULKAN_CHECK(CreateDebugMessengerEXT(instance, &messengerInfo, nullptr, &messenger), "Failed to create debug messenger!");
	}

	Instance::~Instance() {
		if (initialized) {
			DestroyDebugMessengerEXT(instance, messenger, nullptr);

			vkDestroyInstance(instance, nullptr);

			initialized = false;
		}
	}

	void Instance::LogInfo() const {
		uint32_t count;
		vkEnumerateInstanceLayerProperties(&count, nullptr);
		std::vector<VkLayerProperties> layers(count);
		vkEnumerateInstanceLayerProperties(&count, layers.data());

		vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
		std::vector<VkExtensionProperties> extensions(count);
		vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data());

		std::stringstream msg;
		msg << layers.size() << " supported layer(s): ";

		for (const auto& layer : layers) {
			msg << "\n\t" << layer.layerName << " (" << layer.description << ")";
		}

		LOGNFNG(util::Logger::Vulkan, msg.str());

		msg = std::stringstream{};

		msg << extensions.size() << " supported instance extension(s): ";

		for (const auto& extension : extensions) {
			msg << "\n\t" << extension.extensionName << " " << vk::VersionString(extension.specVersion);
		}

		LOGNFNG(util::Logger::Vulkan, msg.str());

		auto requiredExtensions = GetRequiredInstanceExtensions();
		
		msg = std::stringstream{};
		msg << requiredExtensions.size() << " required instance extension(s): ";

		for (const auto& extension : requiredExtensions) {
			msg << "\n\t" << extension;
		}

		LOGNFNG(util::Logger::Vulkan, msg.str());
	}
}