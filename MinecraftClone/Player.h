#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "CollisionBox.h"
#include "Mesh.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

	void MoveRelativeToDirection(float forward, float up, float left);
	void Render(float elapsed, GLuint activeShader) override;
	void Update(float elapsed) override;
	void Rotate(float degrees, glm::vec3 axis) override;
	void SetPosition(glm::vec3 position) override;

	Camera camera;
	CollisionBox collisionBox;
	Mesh* mesh;
	GLuint textureId;
};

