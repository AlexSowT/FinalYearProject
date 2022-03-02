#pragma once
#include <SDL.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include "Camera.h"
#include "ShaderLibrary.h"
#include "Renderer.h"


struct Layer
{
public:
	virtual void Init(){}
	virtual void Update(float deltaTime, bool& running){}
	virtual void Render(ShaderLibrary& shaderLibrary, Renderer& renderer, SDL_Window* window){}
private:
};