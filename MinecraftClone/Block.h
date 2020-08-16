#pragma once
#include "Mesh.h"
#include "GameObject.h"
#include <array>

class Block : public GameObject
{
public:
	Block();
	~Block();
	void Render(float elapsed) override;

	GLuint textureId;
	Mesh* mesh;
};

