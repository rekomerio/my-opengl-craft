#pragma once
#include <glm/glm.hpp>

class CollisionBox
{
public:
	CollisionBox();
	CollisionBox(glm::vec3 size);
	~CollisionBox();

	bool IsCollidingWith(const CollisionBox* other) const;
	glm::vec3 GetCollision(const CollisionBox* other) const;
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;

private:
	glm::vec3 m_Size;
	/// <summary>
	/// Center of the box
	/// </summary>
};

