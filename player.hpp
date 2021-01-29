#ifndef RZ_PLAYER_H
#define RZ_PLAYER_H

// TODO: big mario collisions
// TODO: grow anim and mushroom collision
// TODO: duck if grown
// TODO: run and jump higher
// TODO: die
// TODO: animations
// TODO: slide
// TODO: smb1 physics
// TODO: fireballs
// TODO: invincible
// TODO: command pattern for input actions

class Player
{
public:

	PhysicsComponent physics;

	// the dimensions of the player
	static const int PLAYER_WIDTH = 48;
	static const int PLAYER_HEIGHT = 48;

	// movement
	float mVelocity = 0.0f;
	float mMaxSpeed = 1.5f * 3.0f;
	float mAcceleration = 0.2f * 3.0f;
	float mJumpSpeed = 9.80665 * 3.0f;

	// [B] RUN [A] JUMP
	bool mKeyRunDown = false;
	bool mKeyRunPressed = false;
	bool mKeyJumpDown = false;
	bool mKeyJumpPressed = false;
	bool mKeyLeftDown = false;
	bool mKeyRightDown = false;

	// initializes the variables
	Player()
	{
	}

	void resetEvents()
	{
		mKeyRunPressed = false;
		mKeyJumpPressed = false;
	}

	// handle input
	void handleEvent(SDL_Event& e)
	{
		// if a key was pressed
		if (e.type == SDL_KEYDOWN and e.key.repeat == 0)
		{
			switch (e.key.keysym.sym)
			{
				case SDLK_z:
					mKeyRunDown = true;
					mKeyRunPressed = true;
					break;
				case SDLK_x:
					mKeyJumpDown = true;
					mKeyJumpPressed = true;
					break;
				case SDLK_LEFT:
					mKeyLeftDown = true;
					break;
				case SDLK_RIGHT:
					mKeyRightDown = true;
					break;
			}
		}
		// if a key was released
		else if (e.type == SDL_KEYUP and e.key.repeat == 0)
		{
			switch (e.key.keysym.sym)
			{
				case SDLK_z:
					mKeyRunDown = false;
					break;
				case SDLK_x:
					mKeyJumpDown = false;
					break;
				case SDLK_LEFT:
					mKeyLeftDown = false;
					break;
				case SDLK_RIGHT:
					mKeyRightDown = false;
					break;
			}
		}
	}

	// update the player and check collision against tiles
	void update(double dt)
	{
		if (mKeyRightDown == mKeyLeftDown)
		{
			if (physics.mOnGround) {
				if (mVelocity < 0.0f)
				{
					mVelocity += mAcceleration;
					if (mVelocity >= 0.0f)
					{
						mVelocity = 0.0f;
					}
				}
				else
				{
					mVelocity -= mAcceleration;
					if (mVelocity <= 0.0f)
					{
						mVelocity = 0.0f;
					}
				}
			}
			physics.mSpeed.x = mVelocity;
		}
		else if (mKeyRightDown)
		{
			mVelocity += mAcceleration;
			if (mVelocity > mMaxSpeed)
			{
				mVelocity = mMaxSpeed;
			}
			physics.mSpeed.x = mVelocity;
		}
		else if (mKeyLeftDown)
		{
			mVelocity -= mAcceleration;
			if (mVelocity < -mMaxSpeed)
			{
				mVelocity = -mMaxSpeed;
			}
			physics.mSpeed.x = mVelocity;
		}

		if (physics.mOnGround and mKeyJumpPressed)
		{
			physics.mSpeed.y = -mJumpSpeed;
		}

		physics.mSpeed.y += (0.80665f * 2.5f) * (float)dt;
		physics.mSpeed.y = std::fmin(physics.mSpeed.y, 9.80665 * 3.0f); // max falling speed
		if (not mKeyJumpDown and physics.mSpeed.y < 0.0f)
		{
			physics.mSpeed.y = std::fmax(physics.mSpeed.y, -2.5f * 3.0f);
		}

		physics.update(dt);
		UpdateCamera();

		// floor
		if (physics.mOnGround and not physics.mWasOnGround)
		{
			//std::cout << "floor" << std::endl;
		}
		// ceiling
		if (physics.mAtCeiling and not physics.mWasAtCeiling)
		{
			//std::cout << "ceiling" << std::endl;
			level.mTiles[physics.mCeilingTileY][physics.mCeilingTileX].bump();
		}
		// left
		if (physics.mPushesLeftWall and not physics.mPushedLeftWall)
		{
			//std::cout << "left" << std::endl;
		}
		// right
		if (physics.mPushesRightWall and not physics.mPushedRightWall)
		{
			//std::cout << "right" << std::endl;
		}
	}

	// centers the camera over the player
	void UpdateCamera()
	{
		// center the camera over the player
		camera.x = (physics.mPosition.x + TILE_SIZE / 2) - SCREEN_WIDTH / 2;
		camera.y = TILE_SIZE / 2;

		// keep the camera in bounds
		if (camera.x < 0)
		{
			camera.x = 0;
		}
		if (camera.y < 0)
		{
			camera.y = 0;
		}
		if (camera.x > LEVEL_WIDTH - camera.w)
		{
			camera.x = LEVEL_WIDTH - camera.w;
		}
		if (camera.y > LEVEL_HEIGHT - camera.h)
		{
			camera.y = LEVEL_HEIGHT - camera.h;
		}
	}

	void render()
	{
		data["frame-1"].render(
			(physics.mPosition.x + TILE_SIZE / 2) - camera.x,
			(physics.mPosition.y + TILE_SIZE / 2) - camera.y, 0.2f);
	}
};

Player player;

#endif
