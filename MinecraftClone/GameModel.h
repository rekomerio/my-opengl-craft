#pragma once
#include <glad/glad.h>
#include <array>
#include <glm/gtc/type_ptr.hpp>

class GameModel
{
public:
	GameModel();
	virtual ~GameModel();

	virtual void ApplyModelToProgram(GLuint activeShader);
	virtual void Rotate(float degrees, glm::vec3 axis);
	virtual void SetPosition(glm::vec3 position);
  	inline glm::vec3 GetPosition() const { return m_Position; }
	inline glm::vec3 GetRotation() const { return m_Rotation; }

protected:
	glm::mat4 m_Model;
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
};

