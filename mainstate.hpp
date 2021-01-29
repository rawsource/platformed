#ifndef RZ_MAINSTATE_H
#define RZ_MAINSTATE_H

#include "state.hpp"

class MainState : public State
{
	public:
		void init();
		void cleanup();

		void pause();
		void resume();

		void events(Engine* engine);
		void update(Engine* engine);
		void render(Engine* engine);

		static MainState* instance()
		{
			return &mMainState;
		}

	protected:
		MainState() {}
		~MainState() {}

	private:
		static MainState mMainState;
};

#endif
