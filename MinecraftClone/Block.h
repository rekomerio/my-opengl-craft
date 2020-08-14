#pragma once
#include "Mesh.h"
#include "GameObject.h"

class Block : public GameObject
{
public:
	Block();
	~Block();
	void Render(float elapsed) override;
	bool GenerateBuffers();
	void GenerateMesh(float size);

	std::array<GLuint, 1> VAO;
	std::array<GLuint, 1> VBO;

	GLuint textureId;
	Mesh<12 * 3> mesh;
};

