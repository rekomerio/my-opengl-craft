#include "ChunkHandler.h"

ChunkHandler::ChunkHandler()
{
	m_RenderDistance = 4;
	blockMesh = nullptr;
	m_LastPlayerPosition = glm::vec3(0.0f);
	areCollisionsOn = false;
	threadPool.resize(std::thread::hardware_concurrency());
}

ChunkHandler::~ChunkHandler()
{
	for (auto& chunk : chunks)
		delete chunk;
}

void ChunkHandler::Render(float elapsed, GLuint activeShader, Player& player)
{
	glm::vec3 position = player.GetPosition();
	glm::vec3 chunkSize = glm::vec3(16.0f);

	std::vector<Chunk*> chunksToRender;

	int nRendered = 0;
	for (auto& chunk : chunks)
	{
		if (!chunk) continue;
		if (!IsChunkInRenderDistance(chunk, position)) continue;
		if (!player.camera.IsBoxInView(chunk->GetPosition(), chunkSize)) continue;
		if (chunk->HasOnlyInvisibleCubes()) continue;
			
		chunksToRender.push_back(chunk);
		nRendered++;
	}
	std::cout << "nRendered: "<< nRendered << " Total: " << chunks.size() << "\n";

	CalculateVisibleBlocks(chunksToRender, player);

	for (auto& chunk : chunksToRender)
	{
		chunk->Render(elapsed, activeShader, player);
	}
}

void ChunkHandler::Update(float elapsed, Player& player)
{
	glm::vec3 position = ToChunkPosition(player.GetPosition());

	if (position != m_LastPlayerPosition) 
		MoveChunks(position, player);

	if (areCollisionsOn)
		for (auto& chunk : chunks)
			if (GetDistanceToChunk(chunk, player.GetPosition()) < 17.f)
				for (auto& block : chunk->blocks)
					if (block->type != Block::Type::None && glm::distance(block->GetPosition(), player.GetPosition()) < 2.0f)
						m_CollisionHandler.Handle(&player, block);

	position = ToChunkPosition(player.GetPosition());
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

void ChunkHandler::MoveChunks(const glm::vec3& center, Player& player)
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

	auto task = [&](int id)
	{
		while (index++ < chunksToMove.size() - 1)
		{
			int i = index - 1;
			chunksToMove[i]->SetPosition(emptyPositions[i]);
		}
  	};

	std::vector<std::future<void>> tasks;

	for (size_t i = 0; i < threadPool.size(); i++)
		tasks.push_back(threadPool.push(task));
	
	for (size_t i = 0; i < tasks.size(); i++)
		tasks[i].wait();
}

glm::vec3 ChunkHandler::ToChunkPosition(glm::vec3 position) const
{
	position.x = (int)position.x - (int)position.x % CHUNK_SIZE;
	position.y = (int)position.y - (int)position.y % CHUNK_SIZE;
	position.z = (int)position.z - (int)position.z % CHUNK_SIZE;

	return position;
}

void ChunkHandler::CalculateVisibleBlocks(std::vector<Chunk*>& visibleChunks, Player& player)
{
	if (visibleChunks.size() == 0) return;

	std::mutex mutex;
	std::atomic_int index = 0;

	auto task = [&](int id)
	{
		while (index++ < visibleChunks.size() - 1)
		{
			int i = index.load() - 1;
			visibleChunks[i]->CalculateVisibleBlocks(player);
		}
	};

	std::vector<std::future<void>> tasks;

	for (size_t i = 0; i < threadPool.size(); i++)
		tasks.push_back(threadPool.push(task));

	for (size_t i = 0; i < tasks.size(); i++)
		tasks[i].wait();
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
