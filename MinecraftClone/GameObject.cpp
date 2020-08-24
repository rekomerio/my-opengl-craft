#include "GameObject.h"

GameObject::GameObject()
{
	m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Model = glm::mat4(1.0f);
}

GameObject::~GameObject()
{
	std::vector<GameObject*>::iterator it;
	for (it = children.begin(); it != children.end(); it++)
	{
		GameObject* obj = *it;
		delete obj;
	}
}

void GameObject::Render(float elapsed, GLuint activeShader)
{
	std::vector<GameObject*>::iterator it;
	for (it = children.begin(); it != children.end(); it++)
	{
		// Send model to shader
		GLuint modelLocation = glGetUniformLocation(activeShader, "model");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr((*it)->m_Model));
		(*it)->Render(elapsed, activeShader);
	}
}

void GameObject::Update(float elapsed)
{
	std::vector<GameObject*>::iterator it;
	for (it = children.begin(); it != children.end(); it++)
	{
		(*it)->Update(elapsed);
	}
}

void GameObject::Rotate(float degrees, glm::vec3 axis)
{
	m_Model = glm::rotate(m_Model, glm::radians(degrees), axis);
	m_Rotation = glm::mod(m_Rotation + axis * degrees, 360.0f);
}

void GameObject::SetPosition(glm::vec3 position)
{
	// TODO: Try to rotate with only one call
	m_Model = glm::mat4(1.0f);
	m_Model = glm::translate(m_Model, position);
	m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_Position = position;
}
