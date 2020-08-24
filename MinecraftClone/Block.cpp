#include "Block.h"

Block::Block()
{
    textureId = 0;
    mesh = nullptr; // GameEngine will destroy mesh
}

Block::Block(glm::vec3 position)
{
    Block();
    GameObject::SetPosition(position);
}

Block::~Block()
{
}

void Block::Render(float elapsed, GLuint activeShader)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    mesh->Render();
    // Render children of this Block
    GameObject::Render(elapsed, activeShader);
}

void Block::Update(float elapsed)
{
    // Rotate(elapsed * 10, glm::vec3(0.0f, 1.0f, 0.0f));
    // std::cout << GetRotation().x << std::endl;
    GameObject::Update(elapsed);
}
