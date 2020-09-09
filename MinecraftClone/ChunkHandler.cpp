#include "ChunkHandler.h"

ChunkHandler::ChunkHandler()
{
}

ChunkHandler::~ChunkHandler()
{
	for (auto* chunk : chunks)
		delete chunk;
}

void ChunkHandler::Render(float elapsed, GLuint activeShader, Player& player)
{
	for (auto& chunk : chunks)
	{
		if (!chunk) continue; 
        // + 8 to move the distance measurement to center
		if (glm::distance(chunk->GetPosition() + glm::vec3(8.0f), player.GetPosition()) > (m_RenderDistance * CHUNK_SIZE)) continue;

		chunk->Render(elapsed, activeShader);
	}
}

void ChunkHandler::Update(float elapsed)
{
}

void ChunkHandler::SetRenderDistance(int renderDistance)
{
    m_RenderDistance = renderDistance;
	GenerateChunks();
}

void ChunkHandler::GenerateChunks()
{
	chunks.clear();
	chunks.resize(static_cast<size_t>(m_RenderDistance) * static_cast<size_t>(m_RenderDistance));

	for (size_t x = 0; x < m_RenderDistance; x++)
		for (size_t z = 0; z < m_RenderDistance; z++)
		{
			Chunk* chunk = new Chunk(glm::vec3(x * CHUNK_SIZE, 0.0f, z * CHUNK_SIZE));
			chunk->Generate(blockMesh, blockTextures);
			chunks.push_back(chunk);
		}
}
