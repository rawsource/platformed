#ifndef RZ_ENGINE_H
#define RZ_ENGINE_H

#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class State;

class Engine
{
public:
	bool active;
	double dt;

	void init();
	void cleanup();

	void changeState(State* state);
	void pushState(State* state);
	void popState();

	void processInput();
	void update();
	void render();

	void exit()
	{
		active = false;
	};

	void initSDL();
	void initGL();

private:
	std::vector<State*> states;

	SDL_Window* window = NULL;
	SDL_GLContext context;
};

#endif
