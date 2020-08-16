#pragma once
#include <glad/glad.h>
#include <vector>
#include <array>

struct Vertex {
	Vertex():
		x(0.0f), y(0.0f), z(0.0f), tx(0.0f), ty(0.0f) {}
	Vertex(float x, float y, float z, float tx, float ty): 
		x(x), y(y), z(z), tx(tx), ty(ty) {}

	float x, y, z;
	float tx, ty; // Texture mapping
};

class Mesh
{	
public:
	Mesh();
	~Mesh();
	void GenerateCube(float size);
	void Render() const;
	
	std::array<GLuint, 1> VAO;
	std::array<GLuint, 1> VBO;

	std::vector<Vertex> vertices;
};

