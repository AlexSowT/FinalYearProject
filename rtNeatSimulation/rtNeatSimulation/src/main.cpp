#define GLEW_STATIC
#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
#include<GL/glew.h>
#include<SDL_opengl.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"

// TODO: Make ZOOM work with any aspect ratio
const GLuint WIDTH = 800, HEIGHT = 800;
bool RUNNING = true;
SDL_Event window_event;
Camera camera(glm::vec3(WIDTH / 2, HEIGHT / 2, -700.0f), WIDTH, HEIGHT);

void update(float deltaTime);
void render(Shader shader, SDL_Window* window, GLuint VAO, GLuint texture);

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

	Shader ourShader("./src/vertShader.glsl", "./src/fragShader.glsl");

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions // Colors // Texture Coords
		0.5f, 0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f, // Top Right
		0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // Bottom Right
		- 0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // Bottom Left 
		- 0.5f, 0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f,	1.0f // Top Left
	};

	GLuint indices[] =
	{ // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3 // Second Triangle
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Color - TODO: This can be cleaned up and removed. Keeping here for example
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3* sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR
	);

	SDL_Surface* grass = NULL;
	grass = IMG_Load("./data/textures/grass.png");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, grass->pixels);

	//Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (grass == NULL)
	{
		std::cout << "Error loading grass texture" << std::endl;
	}
	
	float angle = 0;
	//float zoom = 1;
	while (RUNNING)
	{
		// Reference: https://gamedev.stackexchange.com/questions/110825/how-to-calculate-delta-time-with-sdl 
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());

		update(deltaTime);

		render(ourShader, window, VAO, texture);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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

void render(Shader shader, SDL_Window* window, GLuint VAO, GLuint texture)
{
	GLint modelLoc = glGetUniformLocation(shader.Program, "model");
	GLint viewLoc = glGetUniformLocation(shader.Program, "view");
	GLint projectionLoc = glGetUniformLocation(shader.Program, "projection");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewMat()));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMat()));

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shader.Program, "ourTexture"
	), 0);

	shader.Use();

	glBindVertexArray(VAO);

	glm::mat4 model = glm::mat4(1);

	model = glm::translate(model, glm::vec3(100, 0, -100));
	model = glm::scale(model, glm::vec3(70));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	model = glm::mat4(1);

	model = glm::translate(model, glm::vec3(30, 0, -100));
	model = glm::scale(model, glm::vec3(70));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	glBindVertexArray(0);

	SDL_GL_SwapWindow(window);
}

