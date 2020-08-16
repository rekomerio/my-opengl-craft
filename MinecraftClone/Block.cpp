#include "Block.h"

Block::Block()
{
    textureId = 0;
    mesh = nullptr; // GameEngine will destroy mesh
}

Block::~Block()
{
    glDeleteTextures(1, &textureId);
}

void Block::Render(float elapsed)
{
    // glEnable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    mesh->Render();
    // Render children of this Block
    GameObject::Render(elapsed);
}