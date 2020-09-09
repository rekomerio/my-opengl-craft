#include "Player.h"

Player::Player()
{
	mesh = nullptr;
	textureId = 0;
}

Player::~Player()
{
}

void Player::Render(float elapsed, GLuint activeShader)
{
	camera.ApplyToProgram(activeShader);
	/*
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	mesh->Render();

	GameObject::Render(elapsed, activeShader);
	*/
}

void Player::MoveRelativeToDirection(float forward, float up, float left)
{
	camera.MoveRelativeToDirection(forward, up, left);
	glm::vec3 playerPosition(camera.GetPosition());
	playerPosition.y -= 1.0f;
	GameObject::SetPosition(playerPosition);
}

void Player::Update(float elapsed)
{
	GameObject::Update(elapsed);
}

void Player::Rotate(float degrees, glm::vec3 axis)
{
	GameObject::Rotate(degrees, glm::vec3(0.0f, -axis.y, 0.0f));
	camera.AddPitch(degrees * axis.x);
	camera.AddYaw(degrees * axis.y);
}

void Player::SetPosition(glm::vec3 position)
{
	GameObject::SetPosition(position);
	glm::vec3 cameraPosition(position);
	cameraPosition.y += 1.0f;
	camera.SetPosition(cameraPosition);
}
