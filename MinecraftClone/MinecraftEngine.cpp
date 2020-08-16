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
    glfwSetMouseButtonCallback(window, m_OnClick);

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
    block->textureId = GameEngineBase::LoadTexture("textures/container.jpg", GL_RGB);
    
    rootObject->children.push_back(block);

	return true;
}

void MinecraftEngine::Render(float elapsed)
{
	glClearColor(0.3f, 0.3f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    rootObject->Render(elapsed);
}

void MinecraftEngine::Update(float elapsed)
{
    //static_cast<Block*>(rootObject->children[0])->GenerateMesh(sinf(glfwGetTime()));
}


void MinecraftEngine::OnClick(GLFWwindow* window, int button, int action, int mods)
{
    static bool isGlFill = true;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        isGlFill = !isGlFill;

    glPolygonMode(GL_FRONT_AND_BACK, (isGlFill ? GL_LINE : GL_FILL));
}

void MinecraftEngine::m_OnClick(GLFWwindow* window, int button, int action, int mods)
{
    GetInstance()->OnClick(window, button, action, mods);
}