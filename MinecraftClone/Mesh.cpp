#include "Mesh.h"

Mesh::Mesh()
{
    glGenVertexArrays(VAO.size(), VAO.data());
    glGenBuffers(VBO.size(), VBO.data());
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(VAO.size(), VAO.data());
    glDeleteBuffers(VAO.size(), VBO.data());
}

void Mesh::Render() const
{
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, (sizeof(Vertex) * vertices.size()));
}

void Mesh::GenerateCube(float size)
{
    size *= 0.5f;
    
    vertices.clear();

    vertices.push_back(Vertex(-size, -size, -size, 0.0f, 0.0f));
    vertices.push_back(Vertex(size, -size, -size, 1.0f, 0.0f));
    vertices.push_back(Vertex(size, size, -size, 1.0f, 1.0f));
    vertices.push_back(Vertex(size, size, -size, 1.0f, 1.0f));
    vertices.push_back(Vertex(-size, size, -size, 0.0f, 1.0f));
    vertices.push_back(Vertex(-size, -size, -size, 0.0f, 0.0f));

    vertices.push_back(Vertex(-size, -size, size, 0.0f, 0.0f));
    vertices.push_back(Vertex(size, -size, size, 1.0f, 0.0f));
    vertices.push_back(Vertex(size, size, size, 1.0f, 1.0f));
    vertices.push_back(Vertex(size, size, size, 1.0f, 1.0f));
    vertices.push_back(Vertex(-size, size, size, 0.0f, 1.0f));
    vertices.push_back(Vertex(-size, -size, size, 0.0f, 0.0f));

    vertices.push_back(Vertex(-size, size, size, 1.0f, 0.0f));
    vertices.push_back(Vertex(-size, size, -size, 1.0f, 1.0f));
    vertices.push_back(Vertex(-size, -size, -size, 0.0f, 1.0f));
    vertices.push_back(Vertex(-size, -size, -size, 0.0f, 1.0f));
    vertices.push_back(Vertex(-size, -size, size, 0.0f, 0.0f));
    vertices.push_back(Vertex(-size, size, size, 1.0f, 0.0f));

    vertices.push_back(Vertex(size, size, size, 1.0f, 0.0f));
    vertices.push_back(Vertex(size, size, -size, 1.0f, 1.0f));
    vertices.push_back(Vertex(size, -size, -size, 0.0f, 1.0f));
    vertices.push_back(Vertex(size, -size, -size, 0.0f, 1.0f));
    vertices.push_back(Vertex(size, -size, size, 0.0f, 0.0f));
    vertices.push_back(Vertex(size, size, size, 1.0f, 0.0f));

    vertices.push_back(Vertex(-size, -size, -size, 0.0f, 1.0f));
    vertices.push_back(Vertex(size, -size, -size, 1.0f, 1.0f));
    vertices.push_back(Vertex(size, -size, size, 1.0f, 0.0f));
    vertices.push_back(Vertex(size, -size, size, 1.0f, 0.0f));
    vertices.push_back(Vertex(-size, -size, size, 0.0f, 0.0f));
    vertices.push_back(Vertex(-size, -size, -size, 0.0f, 1.0f));

    vertices.push_back(Vertex(-size, size, -size, 0.0f, 1.0f));
    vertices.push_back(Vertex(size, size, -size, 1.0f, 1.0f));
    vertices.push_back(Vertex(size, size, size, 1.0f, 0.0f));
    vertices.push_back(Vertex(size, size, size, 1.0f, 0.0f));
    vertices.push_back(Vertex(-size, size, size, 0.0f, 0.0f));
    vertices.push_back(Vertex(-size, size, -size, 0.0f, 1.0f));

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, (sizeof(Vertex) * vertices.size()), &vertices[0], GL_STATIC_DRAW);
    
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
}