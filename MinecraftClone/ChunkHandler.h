#pragma once
#include <vector>
#include "Chunk.h"
#include "Player.h"
#include "Mesh.h"
#include <iostream>
#include <stack>

class ChunkHandler
{
public:
	ChunkHandler();
	~ChunkHandler();

	void Render(float elapsed, GLuint activeShader, const Player& player);
	void Update(float elapsed, const Player& player);
	void SetRenderDistance(int renderDistance);
	float GetDistanceToChunk(const Chunk* chunk, const glm::vec3& position) const;
	bool IsChunkInRenderDistance(const Chunk* chunk, const glm::vec3& position) const;
	bool IsChunkHere(const Chunk* chunk, const glm::vec3& position) const;
	bool IsAnyChunkHere(const glm::vec3& position) const;
	glm::vec3 FindEmptySlot(const glm::vec3 center) const;
	Chunk* GetFreeChunk(const glm::vec3& position) const;
	Chunk* GetClosestChunk(const glm::vec3& position) const;
	std::vector<glm::vec3> GetCurrentGrid(const glm::vec3 center) const;

	Mesh* blockMesh;
	std::vector<Chunk*> chunks;
	std::vector<GLuint> blockTextures;

private:
	Chunk* m_ClosestChunk;
	void GenerateChunks();
	/// <summary>
	/// How many chunks can be player see	
	/// </summary>
	int m_RenderDistance;
};

