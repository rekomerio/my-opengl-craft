#include "GameModel.h"

GameModel::GameModel()
{
	m_Position = glm::vec3(0.0f);
	m_Rotation = glm::vec3(0.0f);
	m_Model = glm::mat4(1.0f);
}

GameModel::~GameModel()
{
}

void GameModel::ApplyModelToProgram(float elapsed, GLuint activeShader)
{
	GLuint modelLocation = glGetUniformLocation(activeShader, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(m_Model));
}

void GameModel::Rotate(float degrees, glm::vec3 axis)
{
	m_Model = glm::rotate(m_Model, glm::radians(degrees), axis);
	m_Rotation = glm::mod(m_Rotation + axis * degrees, 360.0f);
}

void GameModel::SetPosition(glm::vec3 position)
{
	// TODO: Try to rotate with only one call
	m_Model = glm::mat4(1.0f);
	m_Model = glm::translate(m_Model, position);
	m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_Position = position;
}