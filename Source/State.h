#pragma once

#include "Util\Arena.h"
#include "Util\Vulkan.h"

//Forward declarations
namespace util {

	struct Logger;
	struct Configuration;
	class Time;
}

namespace platform {
	struct Platform;
}

namespace gfx {
	class Renderer;
}

//Structs in this class should be initialized in the order they appear and shutdown in the opposite order!
struct State {
	util::Logger* log;
	util::Configuration* config;
	util::Time* time;
	platform::Platform* platform;
	gfx::Renderer* renderer;

	//Allocators
	util::BumpAllocator allocator;
	util::BumpAllocator tickAllocator;
	util::LongAllocator<vk::MAX_FRAMES_IN_FLIGHT> longAllocator;
};

extern State& global;