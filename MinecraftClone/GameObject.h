#pragma once
#include <vector>
#include <glad/glad.h>
#include <array>
#include <glm/gtc/type_ptr.hpp>
#include "GameModel.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Rotate(float degrees, glm::vec3 axis);
	virtual void SetPosition(glm::vec3 position);
	virtual void Render(float elapsed, GLuint activeShader);
	virtual void Update(float elapsed);
	glm::vec3 GetPosition() const { return m_GameModel.GetPosition(); }
	glm::vec3 GetRotation() const { return m_GameModel.GetRotation(); }

	std::vector<GameObject*> children;
	/// <summary>
	/// Do physics affect this object
	/// </summary>
	bool isStatic;

protected:
	GameModel m_GameModel;
};

