#include "CollisionHandler.h"

CollisionHandler::CollisionHandler()
{
}

CollisionHandler::~CollisionHandler()
{
	for (auto& pair : m_CollisionObjects)
	{
		delete pair.first; // Delete only the collisionBox
	}
		
}

void CollisionHandler::AddCollisionBox(glm::vec3 size, GameObject* gameObject)
{
	m_CollisionObjects.push_back(std::make_pair(new CollisionBox(size), gameObject));
}

void CollisionHandler::Handle(GameObject* a, GameModel* b)
{
	constexpr float minDistance = 0.9995f;

	glm::vec3 aPos = a->GetPosition();
	glm::vec3 bPos = b->GetPosition();
	glm::vec3 collision(aPos.x - bPos.x, aPos.y - bPos.y, aPos.z - bPos.z);

	float stepSize = 0.1f;

	while (abs(collision.x) < minDistance && abs(collision.y) < minDistance && abs(collision.z) < minDistance)
	{	
		float temp;

		float smallestCollision = minDistance - abs(collision.x);

		temp = minDistance - abs(collision.y);
		if (temp < smallestCollision)
			smallestCollision = temp;
		
		temp = minDistance - abs(collision.z);
		if (temp < smallestCollision)
			smallestCollision = temp;

		stepSize = smallestCollision + 0.001f;
		
		aPos.x += ((collision.x > 0) ? stepSize : -stepSize);
		aPos.y += ((collision.y > 0) ? stepSize : -stepSize);
		aPos.z += ((collision.z > 0) ? stepSize : -stepSize);

		collision = glm::vec3(aPos.x - bPos.x, aPos.y - bPos.y, aPos.z - bPos.z);
	}

	if (aPos != a->GetPosition())
		a->SetPosition(aPos);
}
