#include <fstream>
#include <iostream>

#include <glm/vec2.hpp>

#include "lib/json.hpp"
using json = nlohmann::json;

#include "mainstate.hpp"

#include "constants.hpp"
#include "mesh.hpp" // color_buffer, vertex_buffer, data
#include "animation.hpp"
#include "camera.hpp" // camera
#include "coin.hpp"
#include "block.hpp"
#include "level.hpp" // level
#include "physics.hpp"
#include "mushroom.hpp"
#include "player.hpp" // player


MainState MainState::mMainState;

void MainState::init()
{
	Coin::Init();
	Mushroom::Init();

	Mushroom::Add(200, 100);
	Mushroom::Add(600, 100);

	camera.x = 0.0f;
	camera.y = 0.0f;
	camera.w = SCREEN_WIDTH;
	camera.h = SCREEN_HEIGHT;

	loadGFX("tile-1");
	loadGFX("tile-2");
	loadGFX("tile-3");
	loadGFX("tile-4");
	loadGFX("tile-5");
	loadGFX("tile-6");
	loadGFX("coin-bump-1");
	loadGFX("coin-bump-2");
	loadGFX("coin-bump-3");
	loadGFX("coin-bump-4");
	loadGFX("frame-1");
	loadGFX("frame-2");
	loadGFX("item-mushroom-1");
}

void MainState::cleanup()
{
}

void MainState::pause()
{
}

void MainState::resume()
{
}

void MainState::events(Engine* engine)
{
	SDL_Event e;

	player.resetEvents();

	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			engine->exit();
		}
		player.handleEvent(e);
	}
}

void MainState::update(Engine* engine)
{
	// update player
	player.update(engine->dt);

	// update objects
	Mushroom::UpdateAll(engine->dt);
	Coin::UpdateAll(engine->dt);
}

void MainState::render(Engine* engine)
{
	// render level
	level.render();

	// render objects
	Mushroom::RenderAll();
	Coin::RenderAll();

	// render player
	player.render();

	glPushMatrix();

	if (FULLSCREEN)
	{
		glScalef(SCALE_X, SCALE_Y, 1.0f);
		glTranslatef(TRANS_X, TRANS_Y, 0.0f);
	}

	glColor3f(0.42f, 0.55f, 1.0f);
	glRectf(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);

	// render buffer
	glColorPointer(3, GL_FLOAT, 0, &color_buffer[0]);
	glVertexPointer(2, GL_FLOAT, 0, &vertex_buffer[0]);
	glDrawArrays(GL_TRIANGLES, 0, vertex_buffer.size());

	glPopMatrix();

	// clear buffer
	color_buffer.clear();
	vertex_buffer.clear();
}

