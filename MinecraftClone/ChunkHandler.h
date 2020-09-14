#pragma once
#include "Chunk.h"
#include "Player.h"
#include "Mesh.h"
#include "CollisionHandler.h"
#include <vector>
#include <iostream>
#include <stack>
#include <thread>
#include <atomic>
#include <mutex>
#include "threading/ctpl_stl.h"

class ChunkHandler
{
public:
	ChunkHandler();
	~ChunkHandler();

	void Render(float elapsed, GLuint activeShader, Player& player);
	void Update(float elapsed, Player& player);
	void SetRenderDistance(int renderDistance);

	Mesh* blockMesh;
	std::vector<Chunk*> chunks;
	std::vector<GLuint> blockTextures;
	bool areCollisionsOn;

private:
	void GenerateChunks();
	float GetDistanceToChunk(const Chunk* chunk, const glm::vec3& position) const;
	bool IsChunkInRenderDistance(const Chunk* chunk, const glm::vec3& position) const;
	bool IsChunkHere(const Chunk* chunk, const glm::vec3& position) const;
	bool IsAnyChunkHere(const glm::vec3& position) const;
	Chunk* GetClosestChunk(const glm::vec3& position) const;
	void CalculateChunkPositions(const glm::vec3& center, std::vector<glm::vec3>& positions) const;
	void MoveChunks(const glm::vec3& center, Player& player);
	glm::vec3 ToChunkPosition(glm::vec3 position) const;
	void CalculateVisibleBlocks(std::vector<Chunk*>& visibleChunks, Player& player);

	/// <summary>
	/// How many chunks can be player see	
	/// </summary>
	int m_RenderDistance;
	/// <summary>
	/// Position of last chunk player was at
	/// </summary>
	glm::vec3 m_LastPlayerPosition;
	CollisionHandler m_CollisionHandler;
	ctpl::thread_pool threadPool;
};

