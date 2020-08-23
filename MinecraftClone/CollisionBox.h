#pragma once
#include <glm/glm.hpp>

class CollisionBox
{
public:
	CollisionBox();
	~CollisionBox();

	bool IsCollidingWith(const CollisionBox& other) const;

public:
	glm::vec3 m_Size;
	/// <summary>
	/// Center of the box
	/// </summary>
	glm::vec3 m_Position;
};

