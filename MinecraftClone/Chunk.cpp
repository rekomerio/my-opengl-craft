#include "Chunk.h"

Chunk::Chunk(glm::vec3 position)
{
	blocks.resize(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
	m_SortedBlocks.resize(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
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
	glm::vec3 blockSize = glm::vec3(1.0f);

	int counter = 0;
	for (auto& block : m_SortedBlocks)
	{
		if (!block) continue;
		if (block->isHidden) continue;
		
		// TODO: Check is block behind another visible block

		if (block->type != blockType)
		{
			blockType = block->type;

			if (blockType != Block::Type::None)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, (*textures)[blockType % textures->size()]);
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

	for (size_t x = 0; x < CHUNK_SIZE; x++)
		for (size_t y = 0; y < CHUNK_SIZE; y++)
			for (size_t z = 0; z < CHUNK_SIZE; z++)
			{
				Block* block = new Block(glm::vec3(x, y, z) + m_Position);
				block->mesh = mesh;

				blocks[XYZ(x, y, z)] = block;
				m_SortedBlocks[XYZ(x, y, z)] = block;
			}

	isGenerated = true;
}

void Chunk::SetPosition(glm::vec3 position)
{
	seed = ((int)position.x & 0xFF) << 24 | ((int)position.y & 0xFF) << 8 | ((int)position.z & 0xFF);

	for (size_t x = 0; x < CHUNK_SIZE; x++)
		for (size_t y = 0; y < CHUNK_SIZE; y++)
			for (size_t z = 0; z < CHUNK_SIZE; z++)
			{
				int i = XYZ(x, y, z);
				glm::vec3 blockPosition = blocks[i]->GetPosition() + position - m_Position;
				blocks[i]->SetPosition(blockPosition);
				blocks[i]->type = (Block::Type)rndInt(0, 4);
			}

	for (size_t x = 0; x < CHUNK_SIZE; x++)
		for (size_t y = 0; y < CHUNK_SIZE; y++)
			for (size_t z = 0; z < CHUNK_SIZE; z++)
			{
				int i = XYZ(x, y, z);
				if (position.y > -0.1f)
				{
					blocks[i]->type = Block::Type::None;
				}
				else
				{
					Block::Type average = CalculateDominatingBlockType(x, y, z);

					bool generateNew = rndInt(0, 100) < (average == Block::Type::None ? 60 : 20);

					blocks[i]->type = generateNew ? (Block::Type)rndInt(0, 4) : average;

					// TODO: Deeper should have more stone
				}
			}

	SortBlocksByTexture();

	m_Position = position;
}

Block::Type Chunk::CalculateSurroundingAverage(int xx, int yy, int zz) const
{
	int average = 0;
	int nCalculations = 0;
	// 3x3x3 area
	for (int x = -1; x <= 1; x++)
		for (int y = -1; y <= 1; y++)
			for (int z = -1; z <= 1; z++)
			{
				if (x == 0 && y == 0 && z == 0) continue; // No point checking the block itself
				if (xx + x < CHUNK_SIZE && xx + x > 0 && yy + y < CHUNK_SIZE && yy + y > 0 && zz + z < CHUNK_SIZE && zz + z > 0)
				{
					average += blocks[XYZ(xx + x, yy + y, zz + z)]->type;
					nCalculations++;
				}
			}

	if (average > 0)
		average /= nCalculations;

	return (Block::Type)average;
}

Block::Type Chunk::CalculateDominatingBlockType(int xx, int yy, int zz) const
{
	std::unordered_map<Block::Type, int> map;

	for (int x = -1; x <= 1; x++)
		for (int y = -1; y <= 1; y++)
			for (int z = -1; z <= 1; z++)
			{
				//if (x == 0 && y == 0 && z == 0) continue; // No point checking the block itself
				if (xx + x < CHUNK_SIZE && xx + x > 0 && yy + y < CHUNK_SIZE && yy + y > 0 && zz + z < CHUNK_SIZE && zz + z > 0)
				{
					Block::Type type = blocks[XYZ(xx + x, yy + y, zz + z)]->type;

					if (map.find(type) != map.end())
						map.at(type)++;
					else
						map.insert(std::make_pair(type, 1));
				}
			}

	std::pair<Block::Type, int> dominatingPair = std::make_pair(Block::Type::None, 0);
	
	for (auto pair : map)
	{
		if (pair.second > dominatingPair.second)
			dominatingPair = pair;
	}

	return dominatingPair.first;
}

bool Chunk::IsAnyFaceVisible(Block* block) const
{
	glm::vec3 pos = block->GetPosition() - m_Position;
	
	// Cant be sure, so return as true
	if (pos.x + 1 >= CHUNK_SIZE || pos.x - 1 < 0) return true;
	if (pos.y + 1 >= CHUNK_SIZE || pos.y - 1 < 0) return true;
	if (pos.z + 1 >= CHUNK_SIZE || pos.z - 1 < 0) return true;
	
	if (blocks[XYZ(pos.x + 1, pos.y, pos.z)]->type == Block::Type::None) return true;
	if (blocks[XYZ(pos.x - 1, pos.y, pos.z)]->type == Block::Type::None) return true;

	if (blocks[XYZ(pos.x, pos.y + 1, pos.z)]->type == Block::Type::None) return true;
	if (blocks[XYZ(pos.x, pos.y - 1, pos.z)]->type == Block::Type::None) return true;

	if (blocks[XYZ(pos.x, pos.y, pos.z + 1)]->type == Block::Type::None) return true;
	if (blocks[XYZ(pos.x, pos.y, pos.z - 1)]->type == Block::Type::None) return true;

	return false;
}

void Chunk::CalculateVisibleBlocks(Player& player)
{
	glm::vec3 blockSize = glm::vec3(1.0f);
	
	for (auto& block : blocks)
	{
		block->isHidden = (!player.camera.IsBoxInView(block->GetPosition(), blockSize) || !IsAnyFaceVisible(block) || block->type == Block::Type::None);
	}
}

bool Chunk::HasOnlyInvisibleCubes()
{
	for (auto& block : blocks)
		if (block->type != Block::Type::None) 
			return false;
	
	return true;
}

int Chunk::XYZ(int x, int y, int z) const
{
	return x + CHUNK_SIZE * (y + CHUNK_SIZE * z);
}

void Chunk::SortBlocksByTexture()
{
	//std::copy(blocks.begin(), blocks.end(), m_SortedBlocks.begin());
	std::sort(m_SortedBlocks.begin(), m_SortedBlocks.end(), [](Block* a, Block* b) { return a->type < b->type; });
}
