#include "ParticleHandler.h"

ParticleHandler::ParticleHandler(size_t maxParticles)
{
	particles.resize(maxParticles);
	particleIndex = 0;
	position = glm::vec3(0.0f);
	isActive = true;
	particleMesh = nullptr;

	for (size_t i = 0; i < particles.size(); i++)
	{
		particles[i] = Particle();
		particles[i].isAlive = false;
	}
}

void ParticleHandler::Update(float elapsed)
{
	if (!isActive) return;

	for (auto& particle : particles)
	{
		if (particle.isAlive)
			particle.Update(elapsed);
	}
}

void ParticleHandler::Render(float elapsed, GLuint activeShader)
{
	if (!isActive || !particleMesh) return;

	GLuint modelLocation = glGetUniformLocation(activeShader, "model");
	GLuint colorLocation = glGetUniformLocation(activeShader, "color");

	for (auto& particle : particles)
	{
		if (particle.isAlive)
		{
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(particle.m_Model));
			glUniform4f(colorLocation, particle.color.x, particle.color.y, particle.color.z, particle.color.w);
			
			particleMesh->Render();
		}
	}
}

void ParticleHandler::Emit(Particle particle)
{
	particle.SetPosition(ParticleHandler::position);
	particle.lifeLeft = particle.lifeSpan;
	particles[particleIndex] = particle;
	++particleIndex %= particles.size();
}
