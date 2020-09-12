#include "ChunkHandler.h"

ChunkHandler::ChunkHandler()
{
	m_RenderDistance = 4;
	blockMesh = nullptr;
	m_LastPlayerPosition = glm::vec3(0.0f);
}

ChunkHandler::~ChunkHandler()
{
	for (auto& chunk : chunks)
		delete chunk;
}

void ChunkHandler::Render(float elapsed, GLuint activeShader, Player& player)
{
	glm::vec3 position = player.GetPosition();

	int nDiscarded = 0;
	for (auto& chunk : chunks)
	{
		if (!chunk) continue;

		if (!player.camera.IsPointInView(chunk->GetPosition() + 8.0f))
		{
			nDiscarded++;
			//continue;
		}
		if (IsChunkInRenderDistance(chunk, position))
			chunk->Render(elapsed, activeShader, player);
	}
	std::cout << "nDiscarded: "<< nDiscarded << " Total: " << chunks.size() << "\n";
}

void ChunkHandler::Update(float elapsed, Player& player)
{
	glm::vec3 position = ToChunkPosition(player.GetPosition());

	if (position != m_LastPlayerPosition) 
		MoveChunks(position);
	
	m_LastPlayerPosition = position;
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
	// Potential bug on overflow?
	return glm::i32vec3(chunk->GetPosition()) == glm::i32vec3(position);
}

bool ChunkHandler::IsAnyChunkHere(const glm::vec3& position) const
{
	for (auto& chunk : chunks)
	{
		if (IsChunkHere(chunk, position))
			return true;
	}
	return false;
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

void ChunkHandler::CalculateChunkPositions(const glm::vec3& center, std::vector<glm::vec3>& positions) const
{
	glm::vec3 startingCorner = center - glm::vec3(m_RenderDistance * CHUNK_SIZE);
	size_t distance = static_cast<size_t>(m_RenderDistance) * 2;

	for (size_t x = 0; x < distance; x++)
		for (size_t y = 0; y < distance; y++)
			for (size_t z = 0; z < distance; z++)
				positions.push_back(startingCorner + glm::vec3(x * CHUNK_SIZE, y * CHUNK_SIZE, z * CHUNK_SIZE));
}

void ChunkHandler::MoveChunks(const glm::vec3& center)
{
	std::vector<glm::vec3> positions;
	CalculateChunkPositions(center, positions);

	std::vector<glm::vec3> emptyPositions;
	std::vector<Chunk*> chunksToMove;

	for (auto& chunk : chunks)
	{
		bool isChunkInWrongPlace = true;
		for (auto& position : positions)
		{
			if (IsChunkHere(chunk, position))
				isChunkInWrongPlace = false;
		}

		if (isChunkInWrongPlace)
			chunksToMove.push_back(chunk);
	}

	for (auto& position : positions)
	{
		if (!IsAnyChunkHere(position))
			emptyPositions.push_back(position);
	}

	std::atomic_int index = 0;

	auto task = [&]()
	{
		while (index < chunksToMove.size() - 1)
		{
			int i = index++;
			chunksToMove[i]->SetPosition(emptyPositions[i]);
		}
  	};

	std::thread t1(task);
	std::thread t2(task);
	std::thread t3(task);
	std::thread t4(task);
	std::thread t5(task);
	std::thread t6(task);

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
}

glm::vec3 ChunkHandler::ToChunkPosition(glm::vec3 position) const
{
	position.x = (int)position.x - (int)position.x % CHUNK_SIZE;
	position.y = (int)position.y - (int)position.y % CHUNK_SIZE;
	position.z = (int)position.z - (int)position.z % CHUNK_SIZE;

	return position;
}

void ChunkHandler::GenerateChunks()
{
	for (auto& chunk : chunks)
		delete chunk;

	chunks.clear();

	for (size_t x = 0; x < m_RenderDistance * 2; x++)
		for (size_t y = 0; y < m_RenderDistance * 2; y++)
			for (size_t z = 0; z < m_RenderDistance * 2; z++)
			{
				Chunk* chunk = new Chunk(glm::vec3(x * CHUNK_SIZE, y * CHUNK_SIZE, z * CHUNK_SIZE));
				chunk->Generate(blockMesh);
				chunk->textures = &blockTextures;
				chunks.push_back(chunk);
			}
}
