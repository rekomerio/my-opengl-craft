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
	for (auto& block : blocks)
	{
		if (!block) continue;

		glm::vec3 pos = block->GetPosition() - m_Position;
		// TODO: Do some check here to only render blocks that are visible?
		
		// Render outer layer
		if (pos.x > 14 || pos.y > 14 || pos.z > 14 || pos.x < 1 || pos.y < 1 || pos.z < 1)
		block->Render(elapsed, activeShader);
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

void Chunk::Generate(Mesh* mesh, std::vector<GLuint> textures, int seed)
{
	if (isGenerated) return;

	auto xyz = [](int x, int y, int z) { return x + 16 * (y + 16 * z); };

	for (size_t x = 0; x < 16; x++)
		for (size_t y = 0; y < 16; y++)
			for (size_t z = 0; z < 16; z++)
			{
				blocks[xyz(x, y, z)] = new Block(glm::vec3(x, y, z) + m_Position);
				blocks[xyz(x, y, z)]->mesh = mesh;
				blocks[xyz(x, y, z)]->textureId = textures[0];
			}

	isGenerated = true;
}