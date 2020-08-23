#include "CollisionBox.h"

CollisionBox::CollisionBox()
{
}

CollisionBox::~CollisionBox()
{
}

bool CollisionBox::IsCollidingWith(const CollisionBox& other) const
{
	float distance = glm::distance(m_Position, other.m_Position);

	float x = std::fmax(m_Size.x, other.m_Size.x);
	float y = std::fmax(m_Size.y, other.m_Size.y);
	float z = std::fmax(m_Size.z, other.m_Size.z);

	return distance < x || distance < y || distance < z;
}
