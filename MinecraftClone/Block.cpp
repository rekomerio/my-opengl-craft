#include "Block.h"

Block::Block()
{
    textureId = 0;
    mesh = nullptr; // GameEngine will destroy mesh
}

Block::~Block()
{
}

void Block::Render(float elapsed)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Send model to shader, should try to do this in GameObjects render instead
    GLuint modelLocation = glGetUniformLocation(MinecraftEngine::GetInstance()->GetActiveShader(), "model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(m_Model));

    mesh->Render();
    // Render children of this Block
    GameObject::Render(elapsed);
}

void Block::Update(float elapsed)
{
    Rotate(elapsed * 10, glm::vec3(1.0f, 0.0f, 0.0f));
    //std::cout << GetRotation().x << std::endl;
    GameObject::Update(elapsed);
}
