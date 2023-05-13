#pragma once

#include "Types.h"
#include "Std.h"
#include "Result.h"
#include "Math.h"

namespace util {
	struct Configuration {
		static Result<Configuration, std::string> FromFile(const std::string& filename);

		int exit;
		int up;
		int down;
		int right;
		int left;
		int jump;

		uvec2 size;
		u32 monitorIndex;
		bool vsync, fullscreen;

		std::string pipelineCache;
	};
}