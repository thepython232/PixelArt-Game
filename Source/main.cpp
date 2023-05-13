#include "State.h"
#include "Platform\Platform.h"
#include "Util\Configuration.h"
#include "Util\Log.h"
#include "Util\Time.h"
#include "GFX\Renderer.h"

State state;
State& global = state;

int main(UNUSED int argc, UNUSED char* argv[]) {
	util::Logger log{ std::cout, std::cerr };
	state.log = &log;

	auto configResult =
		util::Configuration::FromFile("Resources\\config.toml");
	if (configResult.IsErr()) {
		ERROR(-1, util::Logger::General, configResult.UnwrapErr());
	}
	util::Configuration config = configResult.Unwrap();
	state.config = &config;

	auto startTime = std::chrono::high_resolution_clock::now();
	util::Time time([&startTime]() -> f64 {
		return std::chrono::duration<f64, std::chrono::milliseconds::period>(
			std::chrono::high_resolution_clock::now() - startTime).count();
		});

	state.time = &time;

	platform::Platform platform{};
	state.platform = &platform;
	platform.Init(config);

	gfx::Renderer renderer{};
	state.renderer = &renderer;
	renderer.Init();

	LOG("Hello, World!");

	while (!platform.window->ShouldClose()) {
		time.frame.Begin();

		time.update.Begin();
		platform.keyboard.Prepare();
		platform.mouse.Prepare();
		glfwPollEvents();

		//TODO: update

		if (platform.keyboard["F"].Pressed()) {
			platform.window->ToggleFullscreen();
		}

		time.update.End();

		time.Update();
		for (u32 i = 0; i < time.NumTicks(); i++) {
			time.tick.Begin();
			platform.keyboard.PrepareTick();
			platform.mouse.PrepareTick();

			//TODO; tick

			time.tick.End();
		}

		time.render.Begin();

		//TODO: render
		VkCommandBuffer commandBuffer = renderer.Begin();
		renderer.Composite(commandBuffer);
		renderer.End(commandBuffer);

		time.render.End();

		time.frame.End();
	}

	renderer.Destroy();
	platform.Shutdown();

	LOG("Exitting normally.");

	return 0;
}