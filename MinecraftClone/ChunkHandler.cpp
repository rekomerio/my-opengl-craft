#include "ChunkHandler.h"

ChunkHandler::ChunkHandler()
{
	m_RenderDistance = 4;
	blockMesh = nullptr;
	m_ClosestChunk = nullptr;
}

ChunkHandler::~ChunkHandler()
{
	for (auto& chunk : chunks)
		delete chunk;
}

void ChunkHandler::Render(float elapsed, GLuint activeShader, const Player& player)
{
	glm::vec3 position = player.GetPosition();

	for (auto& chunk : chunks)
	{
		if (!chunk) continue;

		if (IsChunkInRenderDistance(chunk, position))
			chunk->Render(elapsed, activeShader);
	}
}

void ChunkHandler::Update(float elapsed, const Player& player)
{
	glm::vec3 position = player.GetPosition();

	// auto xz = [](int x, int z) { return x + z * CHUNK_SIZE; };

	position.x = (int)position.x - (int)position.x % CHUNK_SIZE;
	position.y = 0; //(int)position.y + (int)position.y % CHUNK_SIZE - 8;
	position.z = (int)position.z - (int)position.z % CHUNK_SIZE;

	std::vector<glm::vec3> grid = GetCurrentGrid(position);
	std::stack<glm::vec3> emptySpots;
	std::stack<Chunk*> chunksToCheck;
	std::stack<Chunk*> chunksToMove;

	for (auto& chunk : chunks)
		chunksToCheck.push(chunk);

	while (!chunksToCheck.empty())
	{
		auto chunk = chunksToCheck.top();
		chunksToCheck.pop();

		bool isChunkInWrongPlace = true;
		for (auto& pos : grid)
		{
			if (IsChunkHere(chunk, pos))
				isChunkInWrongPlace = false;
		}

		if (isChunkInWrongPlace)
			chunksToMove.push(chunk);
	}

	for (auto& pos : grid)
	{
		bool isOccupied = false;
		for (auto& chunk : chunks)
		{
			if (IsChunkHere(chunk, pos))
				isOccupied = true;
		}

		if (!isOccupied) emptySpots.push(pos);
	}

	std::cout << "Chunks to move: " << chunksToMove.size() << '\n';
	std::cout << "Empty spots: " << emptySpots.size() << '\n';

	while (!chunksToMove.empty() && !emptySpots.empty())
	{
		auto chunk = chunksToMove.top();
		chunksToMove.pop();
		auto pos = emptySpots.top();
		emptySpots.pop();

		chunk->SetPosition(pos);
	}
}

void ChunkHandler::SetRenderDistance(int renderDistance)
{
    m_RenderDistance = renderDistance;
	GenerateChunks();
}

float ChunkHandler::GetDistanceToChunk(const Chunk* chunk, const glm::vec3& position) const
{
	// Offset to get measurement of center
	constexpr glm::vec3 chunkOffset(CHUNK_SIZE / 2.0f);
	return glm::distance(chunk->GetPosition() + chunkOffset, position);
}

bool ChunkHandler::IsChunkInRenderDistance(const Chunk* chunk, const glm::vec3& position) const
{
	return GetDistanceToChunk(chunk, position) < (m_RenderDistance * CHUNK_SIZE);
}

bool ChunkHandler::IsChunkHere(const Chunk* chunk, const glm::vec3& position) const
{
	glm::vec3 chunkPos = chunk->GetPosition();
	return (
		(int)chunkPos.x == (int)position.x && 
		(int)chunkPos.y == (int)position.y && 
		(int)chunkPos.z == (int)position.z);
}

bool ChunkHandler::IsAnyChunkHere(const glm::vec3& position) const
{
	for (auto& chunk : chunks)
	{
		glm::vec3 pos = chunk->GetPosition();
		if ((int)pos.x == (int)position.x && (int)pos.y == (int)position.y && (int)pos.z == (int)position.z)
			return true;
	}
	return false;
}

glm::vec3 ChunkHandler::FindEmptySlot(const glm::vec3 center) const
{
	glm::vec3 startingCorner = center - glm::vec3(CHUNK_SIZE * m_RenderDistance, 0, CHUNK_SIZE * m_RenderDistance);

	for (size_t x = 0; x < m_RenderDistance; x++)
		for (size_t z = 0; z < m_RenderDistance; z++)
		{
			if (!IsAnyChunkHere(startingCorner + glm::vec3(x * CHUNK_SIZE, 0.0f, z * CHUNK_SIZE)));
			return startingCorner + glm::vec3(x * CHUNK_SIZE, 0.0f, z * CHUNK_SIZE);
		}

	return glm::vec3();
}

Chunk* ChunkHandler::GetFreeChunk(const glm::vec3& position) const
{
	for (auto& chunk : chunks)
	{
		if (!IsChunkInRenderDistance(chunk, position))
			return chunk;
	}
	return nullptr;
}

Chunk* ChunkHandler::GetClosestChunk(const glm::vec3& position) const
{
	Chunk* closestChunk = chunks[0];
	for (auto& chunk : chunks)
	{
		if (GetDistanceToChunk(chunk, position) < GetDistanceToChunk(closestChunk, position))
			closestChunk = chunk;
	}
	return closestChunk;
}

std::vector<glm::vec3> ChunkHandler::GetCurrentGrid(const glm::vec3 center) const
{
	glm::vec3 startingCorner = center - glm::vec3(m_RenderDistance * CHUNK_SIZE, 0.0f, m_RenderDistance * CHUNK_SIZE);
	std::vector<glm::vec3> positions;

	for (size_t x = 0; x < m_RenderDistance * 2; x++)
		//for (size_t y = 0; y < m_RenderDistance * 2; y++)
			for (size_t z = 0; z < m_RenderDistance * 2; z++)
			{
				positions.push_back(startingCorner + glm::vec3(x * CHUNK_SIZE, 0, z * CHUNK_SIZE));
			}

	return positions;
}

void ChunkHandler::GenerateChunks()
{
	for (auto& chunk : chunks)
		delete chunk;

	chunks.clear();

	for (size_t x = 0; x < m_RenderDistance * 2; x++)
		for (size_t z = 0; z < m_RenderDistance * 2; z++)
		{
			Chunk* chunk = new Chunk(glm::vec3(x * CHUNK_SIZE, 0.0f, z * CHUNK_SIZE));
			chunk->Generate(blockMesh, blockTextures);
			chunks.push_back(chunk);
		}
}
