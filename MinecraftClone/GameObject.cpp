#include "GameObject.h"

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
