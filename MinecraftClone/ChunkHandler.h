#pragma once
#include <vector>
#include "Chunk.h"
#include "Player.h"
#include "Mesh.h"

#define CHUNK_SIZE 16.0f

class ChunkHandler
{
public:
	ChunkHandler();
	~ChunkHandler();

	void Render(float elapsed, GLuint activeShader, Player& player);
	void Update(float elapsed);
	void SetRenderDistance(int renderDistance);
	void GenerateChunks();

	std::vector<Chunk*> chunks;
	Mesh* blockMesh;
	std::vector<GLuint> blockTextures;

private:
	/// <summary>
	/// How far chunks can be from the player to be rendered
	/// </summary>
	int m_RenderDistance;
};

