#pragma once
#include <vector>
#include "CollisionBox.h"
#include "GameObject.h"
#include <iostream>

class CollisionHandler
{
public:
	CollisionHandler();
	~CollisionHandler();
	void AddCollisionBox(glm::vec3 size, GameObject* gameObject = nullptr);
	void Handle();

private:
	std::vector<std::pair<CollisionBox*, GameObject*>> m_CollisionObjects;
};

