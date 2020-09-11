#include "Chunk.h"

Chunk::Chunk(glm::vec3 position)
{
	blocks.resize(16 * 16 * 16);
	m_Position = position;
	isGenerated = false;
}

Chunk::~Chunk()
{
	for (auto& block : blocks)
	{
		delete block;
	}
}

void Chunk::Render(float elapsed, GLuint activeShader)
{
	GLuint modelLocation = glGetUniformLocation(activeShader, "model");

	for (auto& block : blocks)
	{
		if (!block) continue;

		glm::vec3 pos = block->GetPosition() - m_Position;
		// TODO: Do some check here to only render blocks that are visible?
		
		// Render outer layer
		if (pos.x > 14 || pos.y > 14 || pos.z > 14 || pos.x < 1 || pos.y < 1 || pos.z < 1)
		block->Render(elapsed, activeShader, modelLocation);
	}
}

void Chunk::Update(float elapsed)
{
	for (auto& block : blocks)
	{
		if (!block) continue;
		block->Update(elapsed);
	}
}

void Chunk::Generate(Mesh* mesh, std::vector<GLuint>& textures, int seed)
{
	if (isGenerated) return;

	auto xyz = [](int x, int y, int z) { return x + CHUNK_SIZE * (y + CHUNK_SIZE * z); };

	for (size_t x = 0; x < CHUNK_SIZE; x++)
		for (size_t y = 0; y < CHUNK_SIZE; y++)
			for (size_t z = 0; z < CHUNK_SIZE; z++)
			{
				blocks[xyz(x, y, z)] = new Block(glm::vec3(x, y, z) + m_Position);
				blocks[xyz(x, y, z)]->mesh = mesh;
				blocks[xyz(x, y, z)]->textureId = textures[0];
			}

	isGenerated = true;
}

void Chunk::SetPosition(glm::vec3 position)
{
	for (auto& block : blocks)
	{
		if (!block) continue;

		block->SetPosition(block->GetPosition() + position - m_Position);
	}
	m_Position = position;
}
