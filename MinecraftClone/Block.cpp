#include "Block.h"

Block::Block()
{
    mesh = nullptr; // GameEngine will destroy mesh
    type = Block::Type::Dirt;
    isHidden = false;
}

Block::Block(glm::vec3 position)
{
    Block();
    GameModel::SetPosition(position);
}

Block::~Block()
{
}

void Block::Render(float elapsed, GLuint activeShader, GLuint modelLocation)
{
    if (!mesh || type == Block::Type::None) return;

    // GameModel::ApplyModelToProgram(activeShader);
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(m_Model));

    mesh->Render();
}

void Block::Update(float elapsed)
{
    //GameObject::Update(elapsed);
}
