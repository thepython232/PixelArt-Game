#include "Configuration.h"
#include "Platform\Input.h"
#include "toml.hpp"

namespace util {

	Result<Configuration, std::string> Configuration::FromFile(const std::string& filename) {
		using namespace std::literals;

		auto config = toml::parse_file(filename);

		std::string_view exit = config["Controls"]["exit"].value_or("Escape"sv);
		int exitButton = platform::Keyboard::ButtonCode(std::string(exit));

		std::string_view up = config["Controls"]["up"].value_or(""sv);
		if (up == ""sv) {
			return Err("No config for up key!");
		}
		int upButton = platform::Keyboard::ButtonCode(std::string(up));

		std::string_view down = config["Controls"]["down"].value_or(""sv);
		if (down == ""sv) {
			return Err("No config for down key!");
		}
		int downButton = platform::Keyboard::ButtonCode(std::string(down));

		std::string_view right = config["Controls"]["right"].value_or(""sv);
		if (right == ""sv) {
			return Err("No config for right key!");
		}
		int rightButton = platform::Keyboard::ButtonCode(std::string(right));

		std::string_view left = config["Controls"]["left"].value_or(""sv);
		if (left == ""sv) {
			return Err("No config for left key!");
		}
		int leftButton = platform::Keyboard::ButtonCode(std::string(left));

		std::string_view jump = config["Controls"]["jump"].value_or(""sv);
		if (jump == ""sv) {
			return Err("No config for jump key!");
		}
		int jumpButton = platform::Keyboard::ButtonCode(std::string(jump));


		u32 width = config["GFX"]["width"].value_or(0u);
		if (width == 0) {
			return Err("No config for width!");
		}

		u32 height = config["GFX"]["height"].value_or(0u);
		if (height == 0) {
			return Err("No config for height!");
		}

		uvec2 size{ width, height };

		u32 monitor = config["GFX"]["monitor"].value_or(0u);
		bool vsync = config["GFX"]["vsync"].value_or(true);
		bool fullscreen = config["GFX"]["fullscreen"].value_or(false);
		std::string pipelineCache = config["GFX"]["cacheFile"].value_or("pipeline.cache");

		return Configuration{ exitButton, upButton, downButton, rightButton, leftButton, jumpButton, size, monitor, vsync, fullscreen, pipelineCache };
	}
}