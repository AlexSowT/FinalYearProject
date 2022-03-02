#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <map>


// Whole class referenced from opengl sdl book.
class ShaderLibrary
{
public:
	enum class SHADER_TYPE
	{
		COLOR,
		TEXT,
		TEXTURE
	};

	void Init() { }

	void AddShader(const GLchar* vertexPath, const GLchar* fragmentPath, SHADER_TYPE type);

	void Use(SHADER_TYPE type);

	GLuint GetProgramID(SHADER_TYPE type);


private:
	std::map<ShaderLibrary::SHADER_TYPE, GLuint> m_shaders;
};