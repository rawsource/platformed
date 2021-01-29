#include "state.hpp"
#include "constants.hpp"


void Engine::init()
{
	active = true;

	initSDL();
	initGL();
}

void Engine::initSDL()
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);

	if (FULLSCREEN)
	{
		window = SDL_CreateWindow("REV.0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}
	else
	{
		window = SDL_CreateWindow("REV.0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	}

	context = SDL_GL_CreateContext(window);

	if (VSYNC)
		SDL_GL_SetSwapInterval(1);
	else
		SDL_GL_SetSwapInterval(0);

	SDL_CaptureMouse(SDL_TRUE);

	SDL_Cursor *cur = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	SDL_SetCursor(cur);
}

void Engine::initGL()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
//	glFrontFace(GL_CCW);

	glPolygonMode(GL_FRONT, GL_FILL);
}

void Engine::cleanup()
{
	// cleanup the all states
	while (not states.empty())
	{
		states.back()->cleanup();
		states.pop_back();
	}

	// destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	// quit sdl subsystems
	SDL_Quit();
}

void Engine::changeState(State* state)
{
	// cleanup the current state
	if (not states.empty())
	{
		states.back()->cleanup();
		states.pop_back();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->init();
}

void Engine::pushState(State* state)
{
	// pause current state
	if (not states.empty())
	{
		states.back()->pause();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->init();
}

void Engine::popState()
{
	// cleanup the current state
	if (not states.empty())
	{
		states.back()->cleanup();
		states.pop_back();
	}

	// resume previous state
	if (not states.empty())
	{
		states.back()->resume();
	}
}

void Engine::processInput()
{
	// let the state handle events
	states.back()->events(this);
}

void Engine::update()
{
	// let the state update the game
	states.back()->update(this);
}

void Engine::render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// clear color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// enable stencil test
	glEnable(GL_STENCIL_TEST);

	// render stencil rectangle
	if (FULLSCREEN)
	{
		glClearStencil(0);

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glStencilFunc(GL_ALWAYS, 1, 1);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

		glPushMatrix();

		glScalef(SCALE_X, SCALE_Y, 1.0f);
		glTranslatef(TRANS_X, TRANS_Y, 0.0f);
		glRectf(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);

		glPopMatrix();

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilFunc(GL_EQUAL, 1, 1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	}

	// reset modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// enable vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// let the state draw the screen
	states.back()->render(this);

	// disable vertex arrays
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	// disable stencil test
	glDisable(GL_STENCIL_TEST);

	// update screen
	SDL_GL_SwapWindow(window);
}
