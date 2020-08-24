#pragma once
#include "GameObject.h"
#include "Mesh.h"
#include "MinecraftEngine.h"
#include "CollisionBox.h"

class Block : public GameObject
{
public:
	Block();
	Block(glm::vec3 position);
	~Block();

	void Render(float elapsed, GLuint activeShader) override;
	void Update(float elapsed) override;

	GLuint textureId;
	Mesh* mesh;
	CollisionBox collisionBox;
};

