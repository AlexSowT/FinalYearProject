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
#include "Shader.h"
#include "Camera.h"
#include "Renderer.h"

// TODO: Make ZOOM work with any aspect ratio
const GLuint WIDTH = 800, HEIGHT = 800;
bool RUNNING = true;
SDL_Event window_event;
Camera camera(glm::vec3(0, 0, -700.0f), WIDTH, HEIGHT);

void update(float deltaTime);
void render(Shader shaderTexture, Shader shaderColor, SDL_Window* window, std::shared_ptr<GameObject> object, Renderer renderer);


int main(int argc, char* argv[])
{
	// Reference: https://gamedev.stackexchange.com/questions/110825/how-to-calculate-delta-time-with-sdl 
	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;

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

	Shader shaderTexture("./src/vertShader_Texture.glsl", "./src/fragShader_Texture.glsl");
	Shader shaderColor("./src/vertShader_Color.glsl", "./src/fragShader_Color.glsl");
	Renderer renderer;

	std::vector<glm::vec3> v_vertices =
	{
		// Positions // Colors // Texture Coords
		glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.f), // Top Right
		glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.f), // Bottom Right
		glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.f), // Bottom Left
		glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.f) // Top Left
	};

	std::vector<int> v_indices =
	{
		0, 1, 3, // First Triangle
		1, 2, 3 // Second Triangle
	};

	std::shared_ptr<GameObject> square = std::make_shared<GameObject>(v_vertices, v_indices, "./data/textures/grass.png");
	
	while (RUNNING)
	{
		// Reference: https://gamedev.stackexchange.com/questions/110825/how-to-calculate-delta-time-with-sdl 
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());

		update(deltaTime);

		render(shaderTexture, shaderColor, window, square, renderer);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
} 

void update(float deltaTime)
{
	if (SDL_PollEvent(&window_event))
	{
		if (window_event.type == SDL_QUIT)
		{
			RUNNING = false;
		} else if (window_event.type == SDL_KEYDOWN)
		{
			switch (window_event.key.keysym.sym)
			{
			case SDLK_LEFT:
				camera.ProcessKeyboard(LEFT, deltaTime);
				break;
			case SDLK_RIGHT:
				camera.ProcessKeyboard(RIGHT, deltaTime);
				break;

			case SDLK_UP:
				camera.ProcessKeyboard(FORWARD, deltaTime);
				break;

			case SDLK_DOWN:
				camera.ProcessKeyboard(BACKWARD, deltaTime);
				break;

			default:
				break;
			}
		} else if (window_event.type == SDL_MOUSEWHEEL)
		{
			camera.ProcessMouseScroll(window_event.wheel.y);
		}
	}
}

void render(Shader shaderTexture, Shader shaderColor, SDL_Window* window, std::shared_ptr<GameObject> object, Renderer renderer)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//// -- Textuered Rendered -- 
	shaderTexture.Use();
	GLint viewLoc = glGetUniformLocation(shaderTexture.Program, "view");
	GLint projectionLoc = glGetUniformLocation(shaderTexture.Program, "projection");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewMat()));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMat()));

	int grid_width = 10;
	int grid_height = 10;
	for (int i = 0; i < grid_height; ++i)
	{
		for (int j = 0; j < grid_width; ++j)
		{
			glm::mat4 model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(70 * j, 70 * i, -100));
			model = glm::scale(model, glm::vec3(70));
			renderer.render(object, shaderTexture, model);
		}
	}

	renderer.render_text(0, 0, { 1, 1, 1 }, "TEST!!!!", shaderTexture);

	////-- Color Rendender
	shaderColor.Use();
	viewLoc = glGetUniformLocation(shaderColor.Program, "view");
	projectionLoc = glGetUniformLocation(shaderColor.Program, "projection");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewMat()));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMat()));

	glm::mat4 model = glm::mat4(1);
	model = glm::translate(model, glm::vec3(140, 140, -90));
	model = glm::scale(model, glm::vec3(20));
	renderer.render(object, shaderColor, model);

	SDL_GL_SwapWindow(window);
}

