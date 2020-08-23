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

void GameObject::Render(float elapsed)
{
	std::vector<GameObject*>::iterator it;
	for (it = children.begin(); it != children.end(); it++)
	{
		(*it)->Render(elapsed);
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
	// m_Rotation += axis * degrees;
	m_Rotation = glm::mod(m_Rotation + axis * degrees, 360.0f);
}

void GameObject::SetPosition(glm::vec3 position)
{
	m_Model = glm::translate(m_Model, position - m_Position);
	m_Position = position;
}
