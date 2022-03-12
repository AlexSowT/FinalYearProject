#pragma once
#define GLEW_STATIC
#include <SDL_image.h>
#include <glm.hpp>
#include <gl/glew.h>
#include <string>
#include <vector>
#include <stdexcept>

class GameObject
{
public:
	GameObject(std::vector<glm::vec3> vertices, std::vector<int> indices)
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()* sizeof(int), &indices[0], GL_STATIC_DRAW);

		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Color - TODO: This can be cleaned up and removed. Keeping here for example
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// Texture
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);

	}

	GameObject(std::vector<glm::vec3> vertices, std::vector<int> indices, std::string texture_path) : GameObject(vertices, indices) 
	{
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		SDL_Surface* m_surface;
		m_surface = NULL;
		m_surface = IMG_Load(texture_path.c_str());
		
		if (m_surface == NULL)
		{
			throw std::invalid_argument("GameObject::GameObject. Error: Texture filepath could not be loaded");
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_surface->pixels);

		//Generate mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		SDL_FreeSurface(m_surface);

		m_textured = true;
	}


	~GameObject()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);
		glDeleteTextures(1, &m_texture);
	}

	const GLuint& GetVAO() const { return m_VAO; }
	const GLuint& GetTexture() const { return m_texture; }
	const bool GetTextured() const { return m_textured; }

	glm::vec3 GetPosition() const { return m_position; }
	glm::vec3 GetVelocity() const { return m_velocity; }
	glm::vec3 GetAccleration() const { return m_acceleration; }
	glm::vec3 GetForward(){ return m_forward; }

	float GetRotation() { return m_rotation; }

	void SetPosition(glm::vec3 position) { m_position = position; }
	void SetVelocity(glm::vec3 velocity) { m_velocity = velocity; }
	void SetAccleration(glm::vec3 acceleration) { m_acceleration = acceleration; }
	void SetRotation(float rotation)
	{ 
		m_rotation = rotation;
		m_forward.x = glm::cos(m_rotation);
		m_forward.y = glm::sin(m_rotation);
	}

	glm::vec3	m_right{ 1.0f, 0.0f, 0.0f };
	glm::vec3	m_forward{ 1.0f, 0.0f, 0.0f };

private:
	glm::vec3 m_position{ 0.0f };
	glm::vec3 m_velocity{ 0.0f };
	glm::vec3 m_acceleration{ 0.0f };

	glm::vec3 forward{ 0.f, 1.f, 0.f };
	glm::vec3 right{ 1.f, 0.f, 0.f };

	float m_scale{ 1.0f };
	float m_rotation = 0; // In Radians

	GLuint m_VAO, m_VBO, m_EBO;
	GLuint m_texture = NULL;

	bool m_textured = false;

protected:
	GameObject(){}
	void Init(std::vector<glm::vec3> vertices, std::vector<int> indices)
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Color - TODO: This can be cleaned up and removed. Keeping here for example
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// Texture
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}
};