#include "Block.h"

Block::Block()
{
    textureId = 0;
}

Block::~Block()
{
    glDeleteVertexArrays(VAO.size(), VAO.data());
    glDeleteBuffers(VAO.size(), VBO.data());
    glDeleteTextures(1, &textureId);
}

void Block::Render(float elapsed)
{
    // glEnable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());
    // Render children of this Block
    GameObject::Render(elapsed);
}

/// <summary>
/// Generates buffer for vertices
/// </summary>
bool Block::GenerateBuffers()
{
    glGenVertexArrays(VAO.size(), VAO.data());
    glGenBuffers(VBO.size(), VBO.data());

    return true;
}

/// <summary>
/// Buffers must be generated before calling
/// </summary>
void Block::GenerateMesh(float size)
{
    mesh = {
        Vertex(-size, -size, -size, 0.0f, 0.0f),
        Vertex( size, -size, -size, 1.0f, 0.0f),
        Vertex( size,  size, -size, 1.0f, 1.0f),
        Vertex( size,  size, -size, 1.0f, 1.0f),
        Vertex(-size,  size, -size, 0.0f, 1.0f),
        Vertex(-size, -size, -size, 0.0f, 0.0f),

        Vertex(-size, -size,  size, 0.0f, 0.0f),
        Vertex( size, -size,  size, 1.0f, 0.0f),
        Vertex( size,  size,  size, 1.0f, 1.0f),
        Vertex( size,  size,  size, 1.0f, 1.0f),
        Vertex(-size,  size,  size, 0.0f, 1.0f),
        Vertex(-size, -size,  size, 0.0f, 0.0f),

        Vertex(-size,  size,  size, 1.0f, 0.0f),
        Vertex(-size,  size, -size, 1.0f, 1.0f),
        Vertex(-size, -size, -size, 0.0f, 1.0f),
        Vertex(-size, -size, -size, 0.0f, 1.0f),
        Vertex(-size, -size,  size, 0.0f, 0.0f),
        Vertex(-size,  size,  size, 1.0f, 0.0f),

        Vertex( size,  size,  size, 1.0f, 0.0f),
        Vertex( size,  size, -size, 1.0f, 1.0f),
        Vertex( size, -size, -size, 0.0f, 1.0f),
        Vertex( size, -size, -size, 0.0f, 1.0f),
        Vertex( size, -size,  size, 0.0f, 0.0f),
        Vertex( size,  size,  size, 1.0f, 0.0f),

        Vertex(-size, -size, -size, 0.0f, 1.0f),
        Vertex( size, -size, -size, 1.0f, 1.0f),
        Vertex( size, -size,  size, 1.0f, 0.0f),
        Vertex( size, -size,  size, 1.0f, 0.0f),
        Vertex(-size, -size,  size, 0.0f, 0.0f),
        Vertex(-size, -size, -size, 0.0f, 1.0f),

        Vertex(-size,  size, -size, 0.0f, 1.0f),
        Vertex( size,  size, -size, 1.0f, 1.0f),
        Vertex( size,  size,  size, 1.0f, 0.0f),
        Vertex( size,  size,  size, 1.0f, 0.0f),
        Vertex(-size,  size,  size, 0.0f, 0.0f),
        Vertex(-size,  size, -size, 0.0f, 1.0f),
    };

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mesh.vertices), mesh.vertices.data(), GL_STATIC_DRAW);
    // Position

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
}
