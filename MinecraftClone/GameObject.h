#pragma once
#include <vector>
#include <glad/glad.h>

class GameObject
{
public:
	virtual ~GameObject();
	virtual void Render(float elapsed);

	std::vector<GameObject*> children;
};

