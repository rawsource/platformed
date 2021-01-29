#ifndef RZ_PHYSICS_H
#define RZ_PHYSICS_H

class AABB
{
public:

	glm::vec2 center;
	glm::vec2 halfSize;

	AABB() {}

	AABB(glm::vec2 c, glm::vec2 hs)
	{
		center = c;
		halfSize = hs;
	}

	bool Overlaps(AABB other)
	{
		if (std::abs(center.x - other.center.x) > halfSize.x + other.halfSize.x)
		{
			return false;
		}
		if (std::abs(center.y - other.center.y) > halfSize.y + other.halfSize.y)
		{
			return false;
		}
		return true;
	}
};

class PhysicsComponent
{
public:

	// collisions
	AABB mAABB;
	glm::vec2 mAABBOffset;

	// moving object
	glm::vec2 mOldPosition;
	glm::vec2 mPosition;

	glm::vec2 mOldSpeed;
	glm::vec2 mSpeed;

	bool mPushedRightWall = false;
	bool mPushesRightWall = false;

	bool mPushedLeftWall = false;
	bool mPushesLeftWall = false;

	bool mWasOnGround = false;
	bool mOnGround = false;

	bool mWasAtCeiling = false;
	bool mAtCeiling = false;

	// these don't belong here..
	int mCeilingTileX = 0;
	int mCeilingTileY = 0;
	int mGroundTileX = 0;
	int mGroundTileY = 0;

	PhysicsComponent()
	{
		mPosition = glm::vec2(0, 0);
		mAABB.halfSize = glm::vec2(TILE_SIZE / 2, TILE_SIZE / 2);
		mAABBOffset = mAABB.halfSize;
	}

	bool HasCeiling(glm::vec2 position, float& ceilingY)
	{
		glm::vec2 center = position + mAABBOffset;

		// center
		glm::vec2 checkedTile = glm::vec2(center);
		checkedTile.y = checkedTile.y - TILE_SIZE / 2;

		int tileIndexX = level.GetMapTileXAtPoint(checkedTile.x);
		int tileIndexY = level.GetMapTileYAtPoint(checkedTile.y);

		ceilingY = tileIndexY * TILE_SIZE + 24;

		if (level.IsObstacle(tileIndexX, tileIndexY))
		{
			mCeilingTileX = tileIndexX;
			mCeilingTileY = tileIndexY;
			return true;
		}

		return false;
	}

	bool HasGround(glm::vec2 position, float& groundY)
	{
		glm::vec2 center = position + mAABBOffset;

		// left
		glm::vec2 checkedTileLeft = glm::vec2(center);
		checkedTileLeft.x = checkedTileLeft.x + 12;
		checkedTileLeft.y = checkedTileLeft.y + 24; // add + 48

		int tileIndexX1 = level.GetMapTileXAtPoint(checkedTileLeft.x);
		int tileIndexY1 = level.GetMapTileYAtPoint(checkedTileLeft.y);

		// right
		glm::vec2 checkedTileRight = glm::vec2(center);
		checkedTileRight.x = checkedTileRight.x - 12;
		checkedTileRight.y = checkedTileRight.y + 24; // add + 48

		int tileIndexX2 = level.GetMapTileXAtPoint(checkedTileRight.x);
		int tileIndexY2 = level.GetMapTileYAtPoint(checkedTileRight.y);

		groundY = tileIndexY2 * TILE_SIZE - TILE_SIZE - 24; // change 24 to 72

		if (level.IsObstacle(tileIndexX1, tileIndexY1)
				or level.IsObstacle(tileIndexX2, tileIndexY2))
		{
			mGroundTileX = tileIndexX1;
			mGroundTileY = tileIndexY1;
			return true;
		}

		return false;
	}

	bool CollidesWithLeftWall(glm::vec2 position, float& wallX)
	{
		wallX = 0.0f;
		glm::vec2 center = position + mAABBOffset;

		glm::vec2 checkedTile = glm::vec2(center.x - TILE_SIZE / 2, center.y - 12);

		int tileIndexX = level.GetMapTileXAtPoint(checkedTile.x);
		int tileIndexY = level.GetMapTileYAtPoint(checkedTile.y);

		wallX = tileIndexX * TILE_SIZE + TILE_SIZE / 2;

		if (level.IsObstacle(tileIndexX, tileIndexY))
		{
			return true;
		}

		return false;
	}

	bool CollidesWithRightWall(glm::vec2 position, float& wallX)
	{
		wallX = 0.0f;
		glm::vec2 center = position + mAABBOffset;

		glm::vec2 checkedTile = glm::vec2(center.x + TILE_SIZE / 2, center.y - 12);

		int tileIndexX = level.GetMapTileXAtPoint(checkedTile.x);
		int tileIndexY = level.GetMapTileYAtPoint(checkedTile.y);

		wallX = tileIndexX * TILE_SIZE - TILE_SIZE - 24;

		if (level.IsObstacle(tileIndexX, tileIndexY))
		{
			return true;
		}

		return false;
	}

	// updates physics and collisions
	void update(double dt)
	{
		mOldPosition = mPosition;
		mOldSpeed = mSpeed;

		mWasOnGround = mOnGround;
		mPushedRightWall = mPushesRightWall;
		mPushedLeftWall = mPushesLeftWall;
		mWasAtCeiling = mAtCeiling;

		mPosition = mPosition + mSpeed * (float)dt;

		float groundY = 0.0f, ceilingY = 0.0f;
		float rightWallX = 0.0f, leftWallX = 0.0f;

		if (mSpeed.y <= 0.0f and HasCeiling(mPosition, ceilingY))
		{
			mPosition.y = ceilingY;
			mSpeed.y = 0.0f;
			mAtCeiling = true;
		}
		else {
			mAtCeiling = false;
		}

		// --- left
		bool checkAgainLeft = false;

		if (mSpeed.x <= 0.0f and CollidesWithLeftWall(mPosition, leftWallX))
		{
			if (mOldPosition.x - mAABB.halfSize.x + mAABBOffset.x >= leftWallX)
			{
				mPosition.x = leftWallX;
				mSpeed.x = 0.0f;
				mPushesLeftWall = true;
			}
			else
			{
				checkAgainLeft = true;
			}
		}
		else {
			mPushesLeftWall = false;
		}

		// --- right
		bool checkAgainRight = false;

		if (mSpeed.x >= 0.0f and CollidesWithRightWall(mPosition, rightWallX))
		{
			if (mOldPosition.x <= rightWallX)
			{
				mPosition.x = rightWallX;
				mSpeed.x = 0.0f;
				mPushesRightWall = true;
			}
			else
			{
				checkAgainRight = true;
			}
		}
		else {
			mPushesRightWall = false;
		}

		if (mSpeed.y >= 0.0f and HasGround(mPosition, groundY))
		{
			mPosition.y = groundY;
			mSpeed.y = 0.0f;
			mOnGround = true;
		}
		else {
			mOnGround = false;
		}

		if (checkAgainLeft && !mOnGround && !mAtCeiling)
		{
			mPosition.x = leftWallX;
			mSpeed.x = 0.0f;
			mPushesLeftWall = true;
		}
		else if (checkAgainRight && !mOnGround && !mAtCeiling)
		{
			mPosition.x = rightWallX;
			mSpeed.x = 0.0f;
			mPushesRightWall = true;
		}

		mAABB.center = mPosition + mAABBOffset;
	}
};

#endif
