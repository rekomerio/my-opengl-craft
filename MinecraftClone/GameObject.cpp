#include "GameObject.h"

GameObject::GameObject()
{
	isStatic = true;
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

void GameObject::Rotate(float degrees, glm::vec3 axis)
{
	m_GameModel.Rotate(degrees, axis);
}

void GameObject::SetPosition(glm::vec3 position)
{
	m_GameModel.SetPosition(position);
}

void GameObject::Render(float elapsed, GLuint activeShader)
{
	std::vector<GameObject*>::iterator it;
	for (it = children.begin(); it != children.end(); it++)
	{
		(*it)->m_GameModel.ApplyModelToProgram(activeShader);
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