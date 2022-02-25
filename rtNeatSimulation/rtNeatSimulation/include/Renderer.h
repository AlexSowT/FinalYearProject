#pragma once
#define GLEW_STATIC
#include<GL/glew.h>
#include<glm.hpp>
#include<gtc/type_ptr.hpp>
#include <memory>
#include "Shader.h"
#include "GameObject.h"
// TODO: Can this be refactored into a namespace and used staitically? 
class Renderer
{
public:
	Renderer();
	~Renderer();

	void render(const std::shared_ptr<GameObject> object, Shader shader, glm::mat4 transformation);
private:
	//GLuint m_VBO, m_EBO;
};
