#ifndef RZ_MESH_H
#define RZ_MESH_H

struct Vertex
{
	GLfloat x;
	GLfloat y;
};

struct Color
{
	GLfloat r;
	GLfloat g;
	GLfloat b;
};

std::vector<Color> color_buffer;
std::vector<Vertex> vertex_buffer;

struct Mesh
{
	std::vector<Color> colors;
	std::vector<Vertex> vertices;
	inline void render(float x, float y, float scale = 1.0f)
	{
		for (unsigned int i = 0; i < vertices.size(); i++)
		{
			Vertex v = vertices[i];
			v.x *= scale;
			v.y *= scale;
			v.x += x;
			v.y += y;
			vertex_buffer.push_back(v);
		}
		color_buffer.insert(color_buffer.end(), colors.begin(), colors.end());
	}
};

std::map<std::string, Mesh> data;

void loadGFX(std::string name)
{
	std::string filename = "test/" + name + ".json";

	GLfloat SRC_WIDTH = 1920 / 2;
	GLfloat SRC_HEIGHT = 1080 / 2;

	json json_data;
	std::ifstream ifs;

	ifs.open(filename);
	ifs >> json_data;
	ifs.close();

	unsigned int i = 0;
	for (auto& item : json_data)
	{
		Color c;
		c.r = item["r"];
		c.g = item["g"];
		c.b = item["b"];
		data[name].colors.push_back(c);
		Vertex v;
		v.x = item["x"];
		v.x -= SRC_WIDTH;
		v.y = item["y"];
		v.y -= SRC_HEIGHT;
		data[name].vertices.push_back(v);
		i++;
	}
}

#endif
