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
    glfwSetMouseButtonCallback(m_window, m_OnClick);
    glfwSetCursorPosCallback(m_window, m_OnMouseMove);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
}

void MinecraftEngine::Update(float elapsed)
{
}

void MinecraftEngine::OnClick(GLFWwindow* window, int button, int action, int mods)
{
    static bool isGlFill = true;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        isGlFill = !isGlFill;

    glPolygonMode(GL_FRONT_AND_BACK, (isGlFill ? GL_LINE : GL_FILL));
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

    constexpr float sensitivity = 0.1f;
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
