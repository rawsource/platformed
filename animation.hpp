#ifndef RZ_ANIMATION_H
#define RZ_ANIMATION_H

class Frame
{
public:

	std::string mesh;
	unsigned int duration;

	Frame(std::string m, unsigned int d = 0)
	{
		mesh = m;
		duration = d;
	}
};

class Anim
{
public:

	bool loop;
	unsigned int timer;
	unsigned int frame;
	std::vector<Frame> frames;

	Anim()
	{
		frame = 0;
		timer = 0;
		loop = false;
	}

	void update()
	{
		if (timer == frames[frame].duration)
		{
			frame += 1;
			if (frame == frames.size())
			{
				frame = 0;
			}
			timer = 0;
			return;
		}
		timer += 1;
	}

	void render(float x, float y)
	{
		if (frames[frame].duration > 0)
		{
			update();
		}
		data[frames[frame].mesh].render(x, y, 0.2f);
	}
};

#endif
