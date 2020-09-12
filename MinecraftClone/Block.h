#pragma once
#include "GameModel.h"
#include "Mesh.h"

class Block : public GameModel
{
public:
	enum Type {
		Dirt,
		Stone,
		Sand,
		None,
		Cobblestone,
	};

	Block();
	Block(glm::vec3 position);
	~Block();

	void Render(float elapsed, GLuint activeShader, GLuint modelLocation);
	void Update(float elapsed);

	Mesh* mesh;
	Block::Type type;
};

