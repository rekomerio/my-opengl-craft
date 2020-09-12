#pragma once
#include <vector>
#include <glm/common.hpp>
#include <unordered_map>
#include "Block.h"
#include "Player.h"

#define CHUNK_SIZE 16

class Chunk
{
public:
	Chunk(glm::vec3 position);
	~Chunk();

	void Render(float elapsed, GLuint activeShader, Player& player);
	void Update(float elapsed);
	void Generate(Mesh* mesh, int seed = 0);
	void SetPosition(glm::vec3 position);
	Block::Type CalculateSurroundingAverage(int x, int y, int z) const;
	Block::Type CalculateDominatingBlockType(int x, int y, int z) const;
	inline glm::vec3 GetPosition() const { return m_Position; }
	
	std::vector<Block*> blocks;
	std::vector<GLuint>* textures;
private:
	void SortBlocksByTexture();
	std::vector<Block*> m_SortedBlocks;
	glm::vec3 m_Position;
	bool isGenerated;


	/* Random */
	uint32_t seed = 0;

	double rndDouble(double min, double max)
	{
		return ((double)rnd() / (double)(0x7FFFFFFF)) * (max - min) + min;
	}

	int rndInt(int min, int max)
	{
		return (rnd() % (max - min)) + min;
	}

	// Modified from this for 64-bit systems:
	// https://lemire.me/blog/2019/03/19/the-fastest-conventional-random-number-generator-that-can-pass-big-crush/
	// Now I found the link again - Also, check out his blog, it's a fantastic resource!
	uint32_t rnd()
	{
		seed += 0xe120fc15;
		uint64_t tmp;
		tmp = (uint64_t)seed * 0x4a39b70d;
		uint32_t m1 = (tmp >> 32) ^ tmp;
		tmp = (uint64_t)m1 * 0x12fad5c9;
		uint32_t m2 = (tmp >> 32) ^ tmp;
		return m2;
	}
};

