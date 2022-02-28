#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::render(const std::shared_ptr<GameObject> object, Shader shader, glm::mat4 transformation)
{
	shader.Use();

	if (object->get_textured())
	{
		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, object->get_texture());
		glUniform1i(glGetUniformLocation(shader.Program, "ourTexture"
		), 0);
	}

	glBindVertexArray(object->get_VAO());

	GLint modelLoc = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transformation));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Renderer::render_text(float x, float y, SDL_Color color, std::string text, Shader shader)
{
	TTF_Font* font = TTF_OpenFont("./data/fonts/Rubik/static/Rubik-Light.ttf", 24);

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);

	if (textSurface == NULL)
	{
		throw std::invalid_argument("Renderer::render_text. Error: Texture filepath could not be loaded");
	}

	SDL_FreeSurface(textSurface);
	TTF_CloseFont(font);

	GLuint textTexture;

	glGenTextures(1, &textTexture);
	glBindTexture(GL_TEXTURE_2D, textTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, textSurface->pixels);

	const GLfloat quadVertices[] = { -1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f,-1.0f, 0.0f,
	-1.0f,-1.0f, 0.0f
	};

	glVertexPointer(3, GL_FLOAT, 0, quadVertices);
	glDrawArrays(GL_QUADS, 0, 4);

	glBindTexture(GL_TEXTURE_2D, 0);
}
