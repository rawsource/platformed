#ifndef RZ_MUSHROOM_H
#define RZ_MUSHROOM_H

class Mushroom
{
public:
	static const unsigned int MAX_INSTANCES = 16;

	static unsigned int index;
	static std::vector<Mushroom> instances;

	bool active;

	float speed = 2.0f;

	Anim anim;
	PhysicsComponent physics;

	Mushroom()
	{
		active = false;
		physics.mPosition.x = 0;
		physics.mPosition.y = 0;
		anim.frames.push_back(Frame("item-mushroom-1", 0));
	}

	static void Init()
	{
		for (unsigned int i = 0; i < MAX_INSTANCES; i++)
		{
			instances.push_back(Mushroom());
		}
	}

	static void Add(float x, float y)
	{
		instances[index].activate(x, y);
		index++;
		if (index == 16) {
			index = 0;
		}
	}

	static void UpdateAll(double dt)
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

	void activate(float x, float y)
	{
		active = true;
		physics.mPosition.x = x;
		physics.mPosition.y = y;
	}

	void update(double dt)
	{
		physics.mSpeed.x = speed;

		physics.mSpeed.y += (0.80665f * 2.5f) * (float)dt;
		physics.mSpeed.y = std::fmin(physics.mSpeed.y, 9.80665 * 3.0f); // max falling speed

		physics.update(dt);

		// left
		if (physics.mPushesLeftWall and not physics.mPushedLeftWall)
		{
			speed = 2.0f;
		}
		// right
		if (physics.mPushesRightWall and not physics.mPushedRightWall)
		{
			speed = -2.0f;
		}
		// floor
		if (physics.mOnGround)
		{
			auto bumping = level.mTiles[physics.mGroundTileY][physics.mGroundTileX].bumping;
			if (bumping)
			{
				physics.mSpeed.y = -9.80665 * 2.0f;
			}
		}
	}

	inline void render()
	{
		anim.render(
			(physics.mPosition.x + TILE_SIZE / 2) - camera.x,
			(physics.mPosition.y + TILE_SIZE / 2) - camera.y);
	}
};

unsigned int Mushroom::index = 0;
std::vector<Mushroom> Mushroom::instances;

#endif
