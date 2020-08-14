#pragma once
#include "Mesh.h"
#include "GameObject.h"

class Cube : public GameObject
{
public:
	Cube();
	~Cube();
	void Render(float elapsed) override;
	bool Init(float size);

	std::array<GLuint, 1> VAO;
	std::array<GLuint, 1> VBO;

	GLuint textureId;
	Mesh<12 * 3> mesh;
};

