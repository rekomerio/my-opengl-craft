#include "GameObject.h"

GameObject::GameObject()
{
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	model = glm::mat4(1.0f);
	gravity = 0.0f;
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
	model = glm::rotate(model, glm::radians(degrees), axis);
}

void GameObject::SetPosition(glm::vec3 position)
{
	model = glm::translate(model, position - m_Position);
	m_Position = position;
}
