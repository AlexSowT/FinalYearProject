#define GLEW_STATIC
#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
#include<GL/glew.h>
#include<SDL_opengl.h>
#include<SDL_ttf.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include <memory>
#include <vector>
#include "ShaderLibrary.h"
#include "Camera.h"
#include "Renderer.h"
#include "Layer.h"
#include "NEATLayer.h"

// TODO: Make ZOOM work with any aspect ratio
const GLuint WIDTH = 800, HEIGHT = 800;
bool RUNNING = true;
SDL_Event window_event;

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) > 0)
	{
		std::cout << "SDL Init has filed. SDL_Error: " << SDL_GetError() << std::endl;
	}

	// Init PNG Loading
	if (!IMG_Init(IMG_INIT_PNG))
	{
		std::cout << "Image has failed to init. IMG_PNG_Error: " << SDL_GetError() << std::endl;
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_Window* window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;

	GLenum glew_init_error = glewInit();

	if (glew_init_error != GLEW_OK)
	{
		std::cout << "Failed to init GLEW. Error:  "  << glewGetErrorString(glew_init_error) << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	// OPENGL Flag Enablers
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	ShaderLibrary shaderLibrary;
	shaderLibrary.AddShader("./src/vertShader_Texture.glsl", "./src/fragShader_Texture.glsl", ShaderLibrary::SHADER_TYPE::TEXTURE);
	shaderLibrary.AddShader("./src/vertShader_Color.glsl", "./src/fragShader_Color.glsl", ShaderLibrary::SHADER_TYPE::COLOR);
	shaderLibrary.AddShader("./src/vertShader_Text.glsl", "./src/fragShader_Text.glsl", ShaderLibrary::SHADER_TYPE::TEXT);

	Renderer renderer;
	
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0, 0, -700.0f), WIDTH, HEIGHT);

	// Add all layers then init
	std::vector<std::shared_ptr<Layer>> layers;
	layers.push_back(std::make_shared<NEATLayer>(camera));

	for (auto layer : layers)
	{
		layer->Init();
	}

	// Reference: https://gamedev.stackexchange.com/questions/110825/how-to-calculate-delta-time-with-sdl 
	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	float deltaTime = 0;

	// MAIN GAME LOOP
	while (RUNNING)
	{
		// Reference: https://gamedev.stackexchange.com/questions/110825/how-to-calculate-delta-time-with-sdl 
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = (float)((NOW - LAST) * 1000 / (float)SDL_GetPerformanceFrequency());

		// Update to fixed time update for more control

		for (auto layer : layers)
		{
			layer->Update(deltaTime, RUNNING);
			layer->Render(shaderLibrary, renderer, window);
		}
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}