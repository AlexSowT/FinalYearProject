#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::render(const std::shared_ptr<GameObject> object, Shader shader, glm::mat4 transformation)
{
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, object->get_texture());
	glUniform1i(glGetUniformLocation(shader.Program, "ourTexture"
	), 0);

	glBindVertexArray(object->get_VAO());

	GLint modelLoc = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transformation));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}