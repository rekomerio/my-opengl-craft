#pragma once
#include <vector>
#include <glm/common.hpp>
#include "Block.h"

#define CHUNK_SIZE 16

class Chunk
{
public:
	Chunk(glm::vec3 position);
	~Chunk();

	void Render(float elapsed, GLuint activeShader);
	void Update(float elapsed);
	void Generate(Mesh* mesh, std::vector<GLuint>& textures, int seed = 0);
	void SetPosition(glm::vec3 position);
	glm::vec3 GetPosition() const { return m_Position; }

	std::vector<Block*> blocks;
private:
	glm::vec3 m_Position;
	bool isGenerated;
};

