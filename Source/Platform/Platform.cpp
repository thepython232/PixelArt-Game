#include "Platform.h"

namespace platform {

	void Platform::Init(const util::Configuration& config) {
		window = std::make_unique<Window>("GAME", config.size, config.monitorIndex, config.fullscreen);
		window->mouse = &mouse;
		window->keyboard = &keyboard;

		instance = std::make_unique<Instance>();
#ifdef GAME_IS_DEBUG
		instance->LogInfo();
#endif

		device = std::make_unique<Device>(*instance, *window);
#ifdef GAME_IS_DEBUG
		device->LogInfo();
#endif

		swapchain = std::make_unique<Swapchain>(*device, *window);
#ifdef GAME_IS_DEBUG
		swapchain->LogInfo();
#endif
	}

	void Platform::Shutdown() {
		vkDeviceWaitIdle(*device);

		window->Close();
	}
}