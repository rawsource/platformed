#ifndef RZ_COIN_H
#define RZ_COIN_H

class Coin
{
public:
	static const unsigned int MAX_INSTANCES = 16;

	static unsigned int index;
	static std::vector<Coin> instances;

	bool active;

	float x;
	float y;
	int frame;
	float speed;

	Anim anim;

	Coin(float xx, float yy)
	{
		x = xx;
		y = yy;
		frame = 0;
		speed = 18.0f;
		active = false;
		anim.frames.push_back(Frame("coin-bump-1", 2));
		anim.frames.push_back(Frame("coin-bump-2", 4));
		anim.frames.push_back(Frame("coin-bump-3", 2));
		anim.frames.push_back(Frame("coin-bump-4", 2));
	}

	static void Init()
	{
		for (unsigned int i = 0; i < MAX_INSTANCES; i++)
		{
			instances.push_back(Coin(0, 0));
		}
	}

	static void Add(float xx, float yy)
	{
		instances[index].activate(xx, yy);
		index++;
		if (index == 16) {
			index = 0;
		}
	}

	static void UpdateAll(float dt)
	{
		for (unsigned i = 0; i < MAX_INSTANCES; i++)
		{
			if (instances[i].active)
			{
				instances[i].update(dt);
			}
		}
	}

	static void RenderAll()
	{
		for (unsigned i = 0; i < MAX_INSTANCES; i++)
		{
			if (instances[i].active)
			{
				instances[i].render();
			}
		}
	}

	void activate(float xx, float yy)
	{
		x = xx;
		y = yy - TILE_SIZE / 2;
		frame = 0;
		speed = 18.0f;
		active = true;
	}

	void update(float dt)
	{
		frame++;
		y -= speed;
		speed -= 0.9f;
		if (frame == 34)
		{
			active = false;
		}
	}

	inline void render()
	{
		anim.render(x - camera.x, y - camera.y);
	}
};

unsigned int Coin::index = 0;
std::vector<Coin> Coin::instances;

#endif
