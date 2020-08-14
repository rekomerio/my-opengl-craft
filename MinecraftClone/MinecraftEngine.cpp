#include "MinecraftEngine.h"

MinecraftEngine::MinecraftEngine()
{
    rootObject = nullptr;
}

MinecraftEngine::~MinecraftEngine()
{
    delete rootObject;
    glDeleteProgram(shaderId);
}

bool MinecraftEngine::OnCreate()
{
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
    Cube* cube = new Cube();

    if (!cube->Init(0.25f))
        return false;

    cube->textureId = GameEngineBase::LoadTexture("textures/container.jpg", GL_RGB);
    rootObject->children.push_back(cube);

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

}
