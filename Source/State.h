#pragma once

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
};

extern State& global;