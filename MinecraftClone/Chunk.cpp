#include "Chunk.h"

Chunk::Chunk(glm::vec3 position)
{
	blocks.resize(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
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

void Chunk::Render(float elapsed, GLuint activeShader, Player& player)
{
	GLuint modelLocation = glGetUniformLocation(activeShader, "model");

	Block::Type blockType = Block::Type::None;

	for (auto& block : m_SortedBlocks)
	{
		if (!block) continue;
		if (!player.camera.IsPointInView(block->GetPosition())) continue;

		// TODO: Check is block behind another visible block
		auto pos = block->GetPosition();

		if (block->type != blockType)
		{
			blockType = block->type;

			if (blockType != Block::Type::None)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, (*textures)[blockType]);
			}
		}

		block->Render(elapsed, activeShader, modelLocation);
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

void Chunk::Generate(Mesh* mesh, int seed)
{
	if (isGenerated) return;

	auto xyz = [](int x, int y, int z) { return x + CHUNK_SIZE * (y + CHUNK_SIZE * z); };

	for (size_t x = 0; x < CHUNK_SIZE; x++)
		for (size_t y = 0; y < CHUNK_SIZE; y++)
			for (size_t z = 0; z < CHUNK_SIZE; z++)
			{
				blocks[xyz(x, y, z)] = new Block(glm::vec3(x, y, z) + m_Position);
				blocks[xyz(x, y, z)]->mesh = mesh;
			}

	m_SortedBlocks.resize(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
	isGenerated = true;
}

void Chunk::SetPosition(glm::vec3 position)
{
	seed = ((int)position.x & 0xFF) << 24 | ((int)position.y & 0xFFFF) << 8 | ((int)position.z & 0xFF);

	auto xyz = [](int x, int y, int z) { return x + CHUNK_SIZE * (y + CHUNK_SIZE * z); };

	for (size_t x = 0; x < CHUNK_SIZE; x++)
		for (size_t y = 0; y < CHUNK_SIZE; y++)
			for (size_t z = 0; z < CHUNK_SIZE; z++)
			{
				glm::vec3 blockPosition = blocks[xyz(x, y, z)]->GetPosition() + position - m_Position;
				blocks[xyz(x, y, z)]->SetPosition(blockPosition);
				
				bool isVisible = rndInt(0, 100) < (100 - (blockPosition.y * 0.75f));

				if (y > 0) 
					isVisible &= blocks[xyz(x, y - 1, z)]->type != Block::Type::None;

				Block::Type average = CalculateDominatingBlockType(x, y, z);

				bool generateNew = rndInt(0, 100) < (average == Block::Type::None ? 80 : 20);

				// float depth = fmin(fmax(position.y, 0.0f), 100.0f) * 0.03f;

				blocks[xyz(x, y, z)]->type = generateNew ? (Block::Type)rndInt(0, 4) : average;
				
				// TODO: Deeper should have more stone

				blocks[xyz(x, y, z)]->type = (position.y < 16 ? blocks[xyz(x, y, z)]->type : Block::Type::None);
			}

	SortBlocksByTexture();

	m_Position = position;
}

Block::Type Chunk::CalculateSurroundingAverage(int xx, int yy, int zz) const
{
	auto xyz = [](int x, int y, int z) { return x + CHUNK_SIZE * (y + CHUNK_SIZE * z); };

	int average = 0;
	int nCalculations = 0;

	for (int x = -1; x < 1; x++)
		for (int y = -1; y < 1; y++)
			for (int z = -1; z < 1; z++)
				if (xx + x < CHUNK_SIZE && xx + x > 0 && yy + y < CHUNK_SIZE && yy + y > 0 && zz + z < CHUNK_SIZE && zz + z > 0)
				{
					average += blocks[xyz(xx + x, yy + y, zz + z)]->type;
					nCalculations++;
				}

	if (average > 0)
		average /= nCalculations;

	return (Block::Type)average;
}

Block::Type Chunk::CalculateDominatingBlockType(int xx, int yy, int zz) const
{
	auto xyz = [](int x, int y, int z) { return x + CHUNK_SIZE * (y + CHUNK_SIZE * z); };
	std::unordered_map<Block::Type, int> map;

	for (int x = -1; x < 1; x++)
		for (int y = -1; y < 1; y++)
			for (int z = -1; z < 1; z++)
				if (xx + x < CHUNK_SIZE && xx + x > 0 && yy + y < CHUNK_SIZE && yy + y > 0 && zz + z < CHUNK_SIZE && zz + z > 0)
				{
					Block::Type type = blocks[xyz(xx + x, yy + y, zz + z)]->type;

					if (map.find(type) != map.end())
						map.at(type)++;
					else
						map.insert(std::make_pair(type, 1));
				}

	std::pair<Block::Type, int> dominatingPair = std::make_pair(Block::Type::None, 0);
	
	for (auto pair : map)
	{
		if (pair.second > dominatingPair.second)
			dominatingPair = pair;
	}

	return dominatingPair.first;
}

void Chunk::SortBlocksByTexture()
{
	std::copy(blocks.begin(), blocks.end(), m_SortedBlocks.begin());
	std::sort(m_SortedBlocks.begin(), m_SortedBlocks.end(), [](Block* a, Block* b) { return a->type < b->type; });
}
