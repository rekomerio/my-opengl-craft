#include "CollisionBox.h"

CollisionBox::CollisionBox()
{
	m_Size = glm::vec3(0.0f);
	m_Position = glm::vec3(0.0f);
	m_Rotation = glm::vec3(0.0f);
}

CollisionBox::CollisionBox(glm::vec3 size)
{
	CollisionBox::CollisionBox();
	m_Size = size;
}

CollisionBox::~CollisionBox()
{
}

bool CollisionBox::IsCollidingWith(const CollisionBox* other) const
{
	if (other == this) return false;
	
	glm::vec3 distance; 
	distance.x = fabs(other->m_Position.x - m_Position.x);
	distance.y = fabs(other->m_Position.y - m_Position.y);
	distance.z = fabs(other->m_Position.z - m_Position.z);

	bool isCollision = true;

	isCollision &= distance.x < (other->m_Size.x + m_Size.x);
	isCollision &= distance.y < (other->m_Size.y + m_Size.y);
	isCollision &= distance.z < (other->m_Size.z + m_Size.z);

	return isCollision;
}

glm::vec3 CollisionBox::GetCollision(const CollisionBox* other) const
{
	glm::vec3 distance;
	distance.x = ((other->m_Position.x + other->m_Size.x) - (m_Position.x + m_Size.x));
	distance.y = ((other->m_Position.y + other->m_Size.y) - (m_Position.y + m_Size.y));
	distance.z = ((other->m_Position.z + other->m_Size.z) - (m_Position.z + m_Size.z));

	return distance;
}
