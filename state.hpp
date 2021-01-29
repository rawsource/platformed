#ifndef RZ_STATE_H
#define RZ_STATE_H

#include "engine.hpp"

class State
{
public:
	virtual void init() = 0;
	virtual void cleanup() = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual void events(Engine* engine) = 0;
	virtual void update(Engine* engine) = 0;
	virtual void render(Engine* engine) = 0;

	void ChangeState(Engine* engine, State* state)
	{
		engine->changeState(state);
	}

protected:
	State() {};
	~State() {};
};

#endif
