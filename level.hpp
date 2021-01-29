#ifndef RZ_LEVEL_H
#define RZ_LEVEL_H

class Level
{
public:

	int mWidth = 64;
	int mHeight = 16;
	Block mTiles[16][64];

	Level()
	{
		// load map
		json data;
		std::ifstream ifs;
		ifs.open("test/level.json");
		ifs >> data;
		ifs.close();

		int y = 0;
		for (auto& row : data)
		{
			int x = 0;
			for (auto& col : row)
			{
				auto type = static_cast<BlockType>(col);
				mTiles[y][x] = Block(type);
				x++;
			}
			y++;
		}
	}

	int GetMapTileYAtPoint(float y)
	{
		return (int)((y + TILE_SIZE / 2.0f) / TILE_SIZE);
	}

	int GetMapTileXAtPoint(float x)
	{
		return (int)((x + TILE_SIZE / 2.0f) / TILE_SIZE);
	}

	bool IsObstacle(int x, int y)
	{
		if (x < 0 or x >= mWidth or y < 0 or y >= mHeight)
		{
			return true;
		}
		return mTiles[y][x].is_obstacle;
	}

	inline bool insideCameraView(float xpos, float ypos)
	{
		(void) ypos;
		// if (xpos < camera.x or xpos > camera.x + camera.w - TILE_SIZE)
		if (xpos < camera.x - TILE_SIZE or xpos > camera.x + camera.w)
		{
			return false;
		}
		return true;
	}

	void render()
	{
		for (int y = 0; y < mHeight; y++)
		{
			for (int x = 0; x < mWidth; x++)
			{
				float xpos = x * TILE_SIZE;
				float ypos = y * TILE_SIZE;

				if (insideCameraView(xpos, ypos))
				{
					mTiles[y][x].render(xpos - camera.x, ypos - camera.y);
				}
			}
		}
	}
};

Level level;

#endif
