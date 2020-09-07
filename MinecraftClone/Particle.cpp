#include "Particle.h"

Particle::Particle()
{
	isAlive = true;
	mesh = nullptr;
	lifeSpan = 1.0f;
	elapsedLife = 0.0f;
	color = glm::vec4(1.0f);
	velocity = glm::vec3(1.0f);
}

void Particle::Update(float elapsed)
{
	elapsedLife += elapsed;

	if (elapsedLife > lifeSpan)
	{
		isAlive = false;
		return;
	}
		
	GameObject::SetPosition(m_Position + velocity * elapsed);
}

void Particle::Render(float elapsed, GLuint activeShader)
{
	if (!mesh) return;

	GLuint modelLocation = glGetUniformLocation(activeShader, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(m_Model));

	GLuint colorLocation = glGetUniformLocation(activeShader, "color");
	glUniform4f(colorLocation, color.x, color.y, color.z, color.w);
	
	mesh->Render();
}
