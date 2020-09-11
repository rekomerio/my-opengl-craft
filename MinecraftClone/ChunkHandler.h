#pragma once
#include <vector>
#include "Chunk.h"
#include "Player.h"
#include "Mesh.h"
#include <iostream>
#include <stack>
#include <thread>
#include <atomic>
#include <mutex>

class ChunkHandler
{
public:
	ChunkHandler();
	~ChunkHandler();

	void Render(float elapsed, GLuint activeShader, const Player& player);
	void Update(float elapsed, const Player& player);
	void SetRenderDistance(int renderDistance);

	Mesh* blockMesh;
	std::vector<Chunk*> chunks;
	std::vector<GLuint> blockTextures;

private:
	Chunk* m_ClosestChunk;
	void GenerateChunks();
	float GetDistanceToChunk(const Chunk* chunk, const glm::vec3& position) const;
	bool IsChunkInRenderDistance(const Chunk* chunk, const glm::vec3& position) const;
	bool IsChunkHere(const Chunk* chunk, const glm::vec3& position) const;
	bool IsAnyChunkHere(const glm::vec3& position) const;
	Chunk* GetClosestChunk(const glm::vec3& position) const;
	void CalculateChunkPositions(const glm::vec3 center, std::vector<glm::vec3>& positions) const;
	void MoveChunks(const glm::vec3 center);
	glm::vec3 ToChunkPosition(glm::vec3 position) const;

	/// <summary>
	/// How many chunks can be player see	
	/// </summary>
	int m_RenderDistance;
	/// <summary>
	/// Position of last chunk player was at
	/// </summary>
	glm::vec3 m_LastPlayerPosition;
};
