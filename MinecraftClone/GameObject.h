#pragma once
#include <vector>
#include <glad/glad.h>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();
	virtual void Render(float elapsed);
	virtual void Update(float elapsed);
	void Rotate(float degrees, glm::vec3 axis);
	void SetPosition(glm::vec3 position);

	std::vector<GameObject*> children;

	glm::vec3 velocity;
	float gravity;

protected:
	glm::mat4 model;
	glm::vec3 m_Position;
};

