#ifndef RZ_BLOCK_H
#define RZ_BLOCK_H

enum class BlockType
{
	EMPTY = 0,
	STONE = 1,
	BLOCK = 2,
	BRICK = 3,
	SPECIAL = 4,
	INVISIBLE = 5
};

enum class BlockState
{
	NORMAL = 0,
	DISABLED = 1
};

class Block
{
public:

	BlockType type;
	BlockState state;
	std::map<BlockState, Anim> anim;

	// properties
	bool can_bump;
	bool is_obstacle;

	// new
	bool bumping;
	int bump_timer;
	int bump_direction;

	Block()
	{
		bump_timer = 0;
		bumping = false;
		can_bump = false;
		is_obstacle = false;
		bump_direction = 0;
		type = BlockType::EMPTY;
		state = BlockState::NORMAL;
	}

	Block(BlockType t)
	{
		type = t;
		bump_timer = 0;
		bumping = false;
		can_bump = false;
		is_obstacle = false;
		bump_direction = 0;
		state = BlockState::NORMAL;

		if (type == BlockType::EMPTY)
		{
			can_bump = false;
			is_obstacle = false;
		}
		else if (type == BlockType::STONE)
		{
			can_bump = false;
			is_obstacle = true;
			anim[BlockState::NORMAL]
				 .frames.push_back(Frame("tile-1", 0));
		}
		else if (type == BlockType::BLOCK)
		{
			can_bump = false;
			is_obstacle = true;
			anim[BlockState::NORMAL]
				 .frames.push_back(Frame("tile-2", 0));
		}
		else if (type == BlockType::BRICK)
		{
			can_bump = true;
			is_obstacle = true;
			anim[BlockState::NORMAL]
				 .frames.push_back(Frame("tile-3", 0));
		}
		else if (type == BlockType::SPECIAL)
		{
			can_bump = true;
			is_obstacle = true;
			anim[BlockState::NORMAL]
				 .frames.push_back(Frame("tile-4", 30));
			anim[BlockState::NORMAL]
				 .frames.push_back(Frame("tile-5", 5));
			anim[BlockState::NORMAL]
				 .frames.push_back(Frame("tile-6", 5));
			anim[BlockState::NORMAL]
				 .frames.push_back(Frame("tile-5", 5));

			anim[BlockState::DISABLED]
				 .frames.push_back(Frame("tile-2", 0));
		}
		else if (type == BlockType::INVISIBLE)
		{
			can_bump = false;
			is_obstacle = true;
		}
	}

	void bump()
	{
		if (not can_bump)
		{
			return;
		}
		if (type == BlockType::SPECIAL)
		{
			if (state == BlockState::NORMAL)
			{
				bumping = true;
				state = BlockState::DISABLED;
			}
		}
		else {
			bumping = true;
		}
	}

	void render(float x, float y)
	{
		if (type == BlockType::EMPTY)
		{
			return;
		}
		if (bumping)
		{
			// drop coin
			if (bump_timer == 0)
			{
				Coin::Add(x + camera.x, y + camera.y);
			}
			// up
			if (bump_direction == 0)
			{
				bump_timer += 2;
				if (bump_timer == 8)
				{
					bump_direction = 1;
				}
			}
			// down
			else
			{
				bump_timer -= 2;
				if (bump_timer == 0)
				{
					bump_direction = 0;
					bump_timer = 0;
					bumping = false;
				}
			}
		}
		anim[state].render(x, y + (float)-bump_timer * 2);
	}
};

#endif
