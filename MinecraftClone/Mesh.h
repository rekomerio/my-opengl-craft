#pragma once
#include <array>

struct Vertex {
	Vertex():
		x(0.0f), y(0.0f), z(0.0f), tx(0.0f), ty(0.0f) {}
	Vertex(float x, float y, float z, float tx, float ty): 
		x(x), y(y), z(z), tx(tx), ty(ty) {}

	float x, y, z;
	float tx, ty; // Texture mapping
};

template <size_t nVertex>
struct Mesh
{	
	std::array<Vertex, nVertex> vertices;
};

