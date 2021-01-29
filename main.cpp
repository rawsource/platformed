#include <iostream>
#include <chrono>

#include "engine.hpp"
#include "mainstate.hpp"
#include "constants.hpp"

// TODO: improved makefile
// TODO: implement deltatime frame based timers
// TODO: make engine the main global

auto time_prev = std::chrono::high_resolution_clock::now();

double deltatime()
{
	auto time_current = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::ratio<1,1>> time_diff;
	time_diff = (time_current - time_prev);
	time_prev = time_current;
	return time_diff.count() * 60;
}

double dt = 1;

int main(int argc, char* args[])
{
	Engine engine;

	// initialize the engine
	engine.init();

	// load the main state
	engine.changeState(MainState::instance());

	// main loop
	while (engine.active)
	{
		if (not VSYNC)
			dt = deltatime();

		engine.dt = dt;

		engine.processInput();
		engine.update();
		engine.render();
	}

	// free resources and close sdl
	engine.cleanup();

	return 0;
}
