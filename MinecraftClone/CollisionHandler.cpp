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

void CollisionHandler::Handle()
{
	static int counter = 0;

	// First move collision box to same place with game object
	for (auto& pair : m_CollisionObjects)
	{
		if (pair.second == nullptr) continue;

		pair.first->m_Position = pair.second->GetPosition();
		pair.first->m_Rotation = pair.second->GetRotation();
	}
	// Next check the collisions
	for (size_t i = 0; i < m_CollisionObjects.size() - 1; i++)
		for (size_t j = i + 1; j < m_CollisionObjects.size(); j++)
		{
			if (m_CollisionObjects[i].second->isStatic) continue;
			if (m_CollisionObjects[i].first->IsCollidingWith(m_CollisionObjects[j].first))
			{
				//std::cout << "Collision " << ++counter << std::endl;
				// Now move the first object so it no longer collides
				// Move to direction (x, y, z) that collides the least amount
				//glm::vec3 c = m_CollisionObjects[i].first->GetCollision(m_CollisionObjects[j].first);
				//std::cout << "X: " << c.x << " Y: " << c.y << " Z: " << c.z << std::endl;
				glm::vec3 c = m_CollisionObjects[i].first->GetCollision(m_CollisionObjects[j].first);
				std::cout << "X: " << c.x << " Y: " << c.y << " Z: " << c.z << std::endl;
				glm::vec3 cAbs = glm::abs(c);

				if (cAbs.x > cAbs.y && cAbs.x > cAbs.z && fabs(c.x) < 1.0f)
					m_CollisionObjects[i].first->m_Position.x += (c.x > 0.0f) ? (c.x - 1.0f) : (1.0f + c.x);
				else if (cAbs.y > cAbs.x && cAbs.y > cAbs.z && fabs(c.y) < 1.0f)
					m_CollisionObjects[i].first->m_Position.y += (c.y > 0.0f) ? (c.y - 1.0f) : (1.0f + c.y);
				else if (cAbs.z > cAbs.y&& cAbs.z > cAbs.x&& fabs(c.z) < 1.0f)
					m_CollisionObjects[i].first->m_Position.z += (c.z > 0.0f) ? (c.z - 1.0f) : (1.0f + c.z);

				c = m_CollisionObjects[i].first->GetCollision(m_CollisionObjects[j].first);
				std::cout << "X: " << c.x << " Y: " << c.y << " Z: " << c.z << std::endl;
				m_CollisionObjects[i].second->SetPosition(m_CollisionObjects[i].first->m_Position);

			}
		}
}
