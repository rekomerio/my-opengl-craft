#pragma once
#include "GameObject.h"
#include "Camera.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

	void Render(float elapsed, GLuint shaderId);
	void MoveRelativeToDirection(float forward, float up, float left);
	void Update(float elapsed) override;
	void Rotate(float degrees, glm::vec3 axis) override;
	void SetPosition(glm::vec3 position) override;

	Camera camera;
};

