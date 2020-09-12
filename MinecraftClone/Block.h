#pragma once
#include "GameModel.h"
#include "Mesh.h"

enum BlockType {
	Dirt,
	Stone, 
	Sand,
	Cobblestone,
	None,
};

class Block : public GameModel
{
public:
	Block();
	Block(glm::vec3 position);
	~Block();

	void Render(float elapsed, GLuint activeShader, GLuint modelLocation);
	void Update(float elapsed);

	Mesh* mesh;
	BlockType type;
};

