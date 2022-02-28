#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <memory>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdexcept>
#include "Shader.h"
#include "GameObject.h"
// TODO: Can this be refactored into a namespace and used staitically? 
class Renderer
{
public:
	Renderer();
	~Renderer();

	void render(const std::shared_ptr<GameObject> object, Shader shader, glm::mat4 transformation);
	void render_text(float x, float y, SDL_Color color, std::string text, Shader shader);
private:
};
