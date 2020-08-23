#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Render(float elapsed, GLuint shaderId)
{
	GameObject::Render(elapsed);
	camera.ApplyToProgram(shaderId);
}

void Player::MoveRelativeToDirection(float forward, float up, float left)
{
	camera.MoveRelativeToDirection(forward, up, left);
	m_Position = camera.GetPosition();
}

void Player::Update(float elapsed)
{
	GameObject::Update(elapsed);
}

void Player::Rotate(float degrees, glm::vec3 axis)
{
	GameObject::Rotate(degrees, axis);
	camera.AddPitch(degrees * axis.x);
	camera.AddYaw(degrees * axis.y);
}

void Player::SetPosition(glm::vec3 position)
{
	GameObject::SetPosition(position);
	camera.SetPosition(position);
}
