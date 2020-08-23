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
	virtual void Rotate(float degrees, glm::vec3 axis);
	virtual void SetPosition(glm::vec3 position);
	glm::vec3 GetPosition() const { return m_Position; }
	glm::vec3 GetRotation() const { return m_Rotation; }

	std::vector<GameObject*> children;

protected:
	glm::mat4 m_Model;
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
};

