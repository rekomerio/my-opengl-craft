#include "MinecraftEngine.h"

MinecraftEngine::MinecraftEngine()
{
    rootObject = nullptr;
    shaderId = 0;
}

MinecraftEngine::~MinecraftEngine()
{
    delete rootObject;
    glDeleteProgram(shaderId);
}

MinecraftEngine* MinecraftEngine::GetInstance()
{
    static MinecraftEngine* instance = new MinecraftEngine();
    return instance;   
}

bool MinecraftEngine::OnCreate()
{
    glfwSetMouseButtonCallback(m_Window, m_OnClick);
    glfwSetCursorPosCallback(m_Window, m_OnMouseMove);
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, "shaders/vertex.glsl");
    GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, "shaders/fragment.glsl");

    if (!vertexShader)
        return false;
    if (!fragmentShader)
        return false;

    shaderId = CreateShaderProgram(vertexShader, fragmentShader);

    if (!shaderId)
        return false;

    glUseProgram(shaderId);

    rootObject = new GameObject();
    
    // Cube mesh
    Mesh* blockMesh = new Mesh();
    blockMesh->GenerateCube(0.5f);
    meshes.push_back(blockMesh);
    // Block
    Block* block = new Block();
    block->mesh = blockMesh;
    block->textureId = LoadTexture("textures/container.jpg", GL_RGB);
    
    rootObject->children.push_back(block);
    glEnable(GL_DEPTH_TEST);
	return true;
}

void MinecraftEngine::Render(float elapsed)
{
	glClearColor(0.3f, 0.3f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    camera.ApplyToProgram(shaderId);
    rootObject->Render(elapsed);

    char buffer[64];
    // sprintf_s(buffer, "%d", (int)(1.0f / elapsed));
    sprintf_s(buffer, "X: %f Z: %f", camera.GetPosition().x, camera.GetPosition().z);
    glfwSetWindowTitle(m_Window, buffer);
}

void MinecraftEngine::Update(float elapsed)
{
    float cameraSpeed = elapsed * 5.0f;

    if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
        camera.MoveRelativeToDirection(cameraSpeed, 0.0f, 0.0f);
    if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
        camera.MoveRelativeToDirection(-cameraSpeed, 0.0f, 0.0f);
    if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
        camera.MoveRelativeToDirection(0.0f, 0.0f, cameraSpeed);
    if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
        camera.MoveRelativeToDirection(0.0f, 0.0f, -cameraSpeed);
    if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.MoveRelativeToDirection(0.0f, cameraSpeed, 0.0f);
    if (glfwGetKey(m_Window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.MoveRelativeToDirection(0.0f, -cameraSpeed, 0.0f);
}

void MinecraftEngine::OnClick(GLFWwindow* window, int button, int action, int mods)
{
    static bool isGlFill = true;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        isGlFill = !isGlFill;

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
    static glm::dvec2 mouse(-1.0, -1.0);

    if (mouse.x < 0.0 && mouse.y < 0.0)
    {
        mouse.x = x;
        mouse.y = y;
    }

    constexpr float sensitivity = 0.2f;
    glm::dvec2 offset;
    offset.x = x - mouse.x;
    offset.y = mouse.y - y;
    offset *= sensitivity;

    mouse.x = x;
    mouse.y = y;

    camera.AddYaw(offset.x);
    camera.AddPitch(offset.y);
}

void MinecraftEngine::m_OnMouseMove(GLFWwindow* window, double x, double y)
{
    GetInstance()->OnMouseMove(window, x, y);
}
