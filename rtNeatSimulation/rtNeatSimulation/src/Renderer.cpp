#include "Renderer.h"

// TODO: Refernce text_manager.h/cpp in engine.sl (Term 1 ADV Games Tech Engine)
Renderer::Renderer()
{
	//// Init the Text stuff
	//TTF_Font* font = TTF_OpenFont("./data/fonts/Rubik/static/Rubik-Light.ttf", 24);

	if (FT_Init_FreeType(&m_ft))
	{
		//TODO: Log an error 
		//LOG_CORE_ERROR("[text] Could not init FreeType Library");
	}

	FT_Face face;
	if (FT_New_Face(m_ft, "./data/fonts/Rubik/static/Rubik-Light.ttf", 0, &face))
	{
	//TODO: Log an error 
	//LOG_CORE_ERROR("[text] Failed to load font 'arial.ttf'");
	}
	else
	{
		FT_Set_Pixel_Sizes(face, 0, 48);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

		for (GLubyte c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				//TODO: Log an error 
				//LOG_CORE_ERROR("[text] Failed to load Glyph");
				continue;
			}
			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			character chara = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				(uint32_t)face->glyph->advance.x
			};
			m_characters.insert(std::pair<GLchar, character>(c, chara));
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}
}

Renderer::~Renderer()
{
}

void Renderer::render(ShaderLibrary shaderLibrary, ShaderLibrary::SHADER_TYPE type, const std::shared_ptr<GameObject> object, glm::mat4 transformation)
{
	shaderLibrary.Use(type);

	if (object->get_textured())
	{
		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, object->get_texture());
		glUniform1i(glGetUniformLocation(shaderLibrary.GetProgramID(type), "ourTexture"
		), 0);
	}

	glBindVertexArray(object->get_VAO());

	GLint modelLoc = glGetUniformLocation(shaderLibrary.GetProgramID(type), "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transformation));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Renderer::render_text(ShaderLibrary shaderLibrary, ShaderLibrary::SHADER_TYPE type, std::string text, float x, float y, float scale, glm::vec4 colour)
{
	shaderLibrary.Use(type);
	const int32_t uniformLocation = glGetUniformLocation(shaderLibrary.GetProgramID(type), "textColor");
	glUniform4f(uniformLocation, colour.x, colour.y, colour.z, colour.w);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		character ch = m_characters[*c];

		GLfloat xpos = x + ch.bearing.x * scale;
		GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.texture_id);

		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
