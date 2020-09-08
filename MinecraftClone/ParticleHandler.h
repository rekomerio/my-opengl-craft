#pragma once
#include "Particle.h"
#include "Camera.h"
#include "Mesh.h"
#include <glm/glm.hpp>

class ParticleHandler
{
public:
	ParticleHandler(size_t maxParticles);
	void Update(float elapsed);
	void Render(float elapsed, GLuint activeShader);
	void Emit(Particle particle);

	bool isActive; 
	glm::vec3 position; 
	GLuint particleTexture;
	Mesh* particleMesh;
private:
	std::vector<Particle> particles;
	int particleIndex;
};

