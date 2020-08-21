#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "Mesh.h"

class GameEngineBase
{
public:
	GameEngineBase();
	virtual ~GameEngineBase();

	virtual bool OnCreate() = 0;
	virtual void Render(float elapsed) = 0;
	virtual void Update(float elapsed) = 0;
	
	void Run(int width, int height, const char* title);
	void Close();

	static GLuint LoadTexture(std::string path, GLuint colorFormat);
	static GLuint CreateShader(GLuint shaderType, std::string source);
	static GLuint CreateShaderProgram(GLuint vertexShader, GLuint fragmentShader);

protected:
	GLFWwindow* m_Window;
	std::vector<Mesh*> meshes;
};

