#include "MinecraftEngine.h"

MinecraftEngine::MinecraftEngine() : particleHandler(1000)
{
    rootObject = nullptr;
    player = nullptr;
    activeShader = 0;
}

MinecraftEngine::~MinecraftEngine()
{
    delete rootObject;
    glDeleteProgram(activeShader);
}

MinecraftEngine* MinecraftEngine::GetInstance()
{
    static MinecraftEngine* instance = new MinecraftEngine();
    return instance;   
}

GLuint& MinecraftEngine::GetActiveShader()
{
    return activeShader;
}

bool MinecraftEngine::OnCreate()
{
    glfwSetMouseButtonCallback(m_Window, m_OnClick);
    glfwSetCursorPosCallback(m_Window, m_OnMouseMove);
    glfwSetFramebufferSizeCallback(m_Window, m_OnResize);
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, "shaders/vertex.glsl");
    GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, "shaders/fragment.glsl");

    if (!vertexShader)
        return false;
    if (!fragmentShader)
        return false;

    activeShader = CreateShaderProgram(vertexShader, fragmentShader);

    if (!activeShader)
        return false;

    rootObject = new GameObject();
    
    // Cube mesh
    Mesh* blockMesh = new Mesh();
    blockMesh->GenerateCube(1.0f);
    meshes.push_back(blockMesh);

    // Particle mesh
    Mesh* particleMesh = new Mesh();
    particleMesh->GenerateCube(0.2f);
    particleHandler.particleMesh = particleMesh;
    meshes.push_back(particleMesh);

    particleHandler.position = glm::vec3(-1.0f, 0.0f, -1.0f);

    // Block
    GLuint texture = LoadTexture("textures/container.jpg", GL_RGB);
    textures.push_back(texture);

    player = new Player();
    player->isStatic = false;
    collisionHandler.AddCollisionBox(glm::vec3(0.5f), player);
    // Player must be first in list so it gets rendered first and camera applied to shader
    rootObject->children.push_back(player);

    {
        Chunk* chunk = new Chunk(glm::vec3(0.0f));
        chunk->Generate(blockMesh, textures);
        chunks.push_back(chunk);
    }
    {
        Chunk* chunk = new Chunk(glm::vec3(17.0f, 0.0f, 0.0f));
        chunk->Generate(blockMesh, textures);
        chunks.push_back(chunk);
    }
    {
        Chunk* chunk = new Chunk(glm::vec3(17.0f, 0.0f, 17.0f));
        chunk->Generate(blockMesh, textures);
        chunks.push_back(chunk);
    }
    {
        Chunk* chunk = new Chunk(glm::vec3(0.0f, 0.0f, 17.0f));
        chunk->Generate(blockMesh, textures);
        chunks.push_back(chunk);
    }

    player->mesh = blockMesh;
    player->textureId = texture;
    player->SetPosition(glm::vec3(-3.0f, 0.0f, 0.0f));

    glUseProgram(activeShader);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

void MinecraftEngine::Render(float elapsed)
{
	glClearColor(0.0f, 0.5f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniform1i(glGetUniformLocation(activeShader, "useTexture"), 1);
    
    rootObject->Render(elapsed, activeShader);

    for (auto& chunk : chunks)
    {
        chunk->Render(elapsed, activeShader);
    }

    glUniform1i(glGetUniformLocation(activeShader, "useTexture"), 0);
    particleHandler.Render(elapsed, activeShader);

    char buffer[128];
    // sprintf_s(buffer, "%d", (int)(1.0f / elapsed));
    glm::vec3 pos = player->GetPosition();
    sprintf_s(buffer, "X: %f Y:%f Z: %f", pos.x, pos.y, pos.z);
    glfwSetWindowTitle(m_Window, buffer);
}

void MinecraftEngine::Update(float elapsed)
{
    float playerSpeed = elapsed * 7.5f;

    auto randf = [](float min, float max) { return ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * (max - min)) + min; };

    bool doubleSpeed = glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;

    if (doubleSpeed) playerSpeed *= 2.0f;

    if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
        player->MoveRelativeToDirection(playerSpeed, 0.0f, 0.0f);
    if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
        player->MoveRelativeToDirection(-playerSpeed, 0.0f, 0.0f);
    if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
        player->MoveRelativeToDirection(0.0f, 0.0f, playerSpeed);
    if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
        player->MoveRelativeToDirection(0.0f, 0.0f, -playerSpeed);
    if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS)
        player->MoveRelativeToDirection(0.0f, playerSpeed, 0.0f);
    if (glfwGetKey(m_Window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        player->MoveRelativeToDirection(0.0f, -playerSpeed, 0.0f);
    
    {
        Particle particle;
        particle.velocity = glm::vec3(randf(-1.0f, 1.0f), randf(1.0f, 3.0f), randf(-1.0f, 1.0f));
        particle.lifeSpan = 2.0f;
        particle.color = glm::vec4(randf(0.75f, 1.0f), randf(0.75f, 1.0f), randf(0.0f, 0.1f), 1.0f);
        particleHandler.Emit(particle);
    }

    rootObject->Update(elapsed);
    for (auto& chunk : chunks)
    {
        chunk->Render(elapsed, activeShader);
    }
    particleHandler.Update(elapsed);
    // collisionHandler.Handle();
}

void MinecraftEngine::OnClick(GLFWwindow* window, int button, int action, int mods)
{
    static bool isGlFill = true;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        isGlFill = !isGlFill;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        particleHandler.isActive = !particleHandler.isActive;

    glPolygonMode(GL_FRONT_AND_BACK, (!isGlFill ? GL_LINE : GL_FILL));
}

/// <summary>
/// Forwards arguments to OnClick
/// </summary>
inline void MinecraftEngine::m_OnClick(GLFWwindow* window, int button, int action, int mods)
{
    GetInstance()->OnClick(window, button, action, mods);
}

void MinecraftEngine::OnMouseMove(GLFWwindow* window, double x, double y)
{
    static glm::vec2 mouse(-1.0, -1.0);

    if (mouse.x < 0.0 && mouse.y < 0.0)
    {
        mouse.x = (float)x;
        mouse.y = (float)y;
    }

    constexpr float sensitivity = 0.2f;
    glm::vec2 offset;
    offset.x = x - mouse.x;
    offset.y = mouse.y - y;
    offset *= sensitivity;

    mouse.x = x;
    mouse.y = y;

    player->camera.AddPitch(offset.y);
    player->Rotate(offset.x, glm::vec3(0.0f, 1.0f, 0.0f));
}

void MinecraftEngine::m_OnMouseMove(GLFWwindow* window, double x, double y)
{
    GetInstance()->OnMouseMove(window, x, y);
}


void MinecraftEngine::OnResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    player->camera.SetAspectRatio(static_cast<float>(width / height));
}

void MinecraftEngine::m_OnResize(GLFWwindow* window, int width, int height)
{
    GetInstance()->OnResize(window, width, height);
}