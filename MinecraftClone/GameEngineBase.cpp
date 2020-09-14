#include "GameEngineBase.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


GameEngineBase::GameEngineBase()
{
	m_Window = nullptr;
	// threadPool.resize(std::thread::hardware_concurrency());
}

GameEngineBase::~GameEngineBase()
{
	{
		std::vector<Mesh*>::iterator it;
		for (it = meshes.begin(); it != meshes.end(); it++)
			delete (*it);
	}
	{
		std::vector<GLuint>::iterator it;
		for (it = textures.begin(); it != textures.end(); it++)
			glDeleteTextures(1, &(*it));
	}
}

void GameEngineBase::Run(int width, int height, const char* title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (m_Window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	if (!OnCreate())
	{
		std::cout << "OnCreate failed" << std::endl;
		return;
	}

	float currentTime;
	float lastRender = 0.0f;
	float lastUpdate = 0.0f;

	while (!glfwWindowShouldClose(m_Window))
	{
		currentTime = (float)glfwGetTime();
		Update(currentTime - lastUpdate);
		lastUpdate = currentTime;

		currentTime = (float)glfwGetTime();
		Render(currentTime - lastRender);
		lastRender = currentTime;

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}

void GameEngineBase::Close()
{
	glfwSetWindowShouldClose(m_Window, true);
}

GLuint GameEngineBase::LoadTexture(std::string path, GLuint colorFormat)
{
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	
	stbi_image_free(data);
	return textureId;
}

GLuint GameEngineBase::CreateShader(GLuint shaderType, std::string source)
{
	std::stringstream stream;
	std::string shaderRaw;
	std::ifstream file;

	file.open(source);
	stream << file.rdbuf();
	file.close();

	shaderRaw = stream.str();
	const char* buf = shaderRaw.c_str();

	GLuint shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &buf, NULL);
	glCompileShader(shaderId);

	int isSuccess;
	char infoLog[512];

	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isSuccess);

	if (!isSuccess)
	{
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		return 0;
	};

	return shaderId;
}

GLuint GameEngineBase::CreateShaderProgram(GLuint vertexShader, GLuint fragmentShader)
{
	int isSuccess;
	char infoLog[512];
	GLuint id = glCreateProgram();

	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &isSuccess);

	if (!isSuccess)
	{
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
		return 0;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return id;
}
