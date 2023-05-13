#pragma once

#include "Window.h"
#include "Instance.h"
#include "Device.h"
#include "Input.h"
#include "Util\Configuration.h"
#include "Swapchain.h"

namespace platform {
	
	struct Platform {
		std::unique_ptr<Window> window;
		std::unique_ptr<Instance> instance;
		std::unique_ptr<Device> device;
		std::unique_ptr<Swapchain> swapchain;

		Keyboard keyboard;
		Mouse mouse;

		void Init(const util::Configuration& config);
		void Shutdown();
	};
}