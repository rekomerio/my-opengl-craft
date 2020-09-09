#include "Block.h"

Block::Block()
{
    textureId = 0;
    mesh = nullptr; // GameEngine will destroy mesh
}

Block::Block(glm::vec3 position)
{
    Block();
    GameModel::SetPosition(position);
}

Block::~Block()
{
}

void Block::Render(float elapsed, GLuint activeShader)
{
    // TODO: Sort game objects by texture so less OpenGl calls will be made
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    GameModel::ApplyModelToProgram(activeShader);

    mesh->Render();
    // Render children of this Block
   // GameObject::Render(elapsed, activeShader);
}

void Block::Update(float elapsed)
{
    //GameObject::Update(elapsed);
}
