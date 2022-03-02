#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <SDL.h>
#include <SDL_ttf.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <memory>
#include <string>
#include <stdexcept>
#include <map>
#include "ShaderLibrary.h"
#include "GameObject.h"

struct character
{
	uint32_t     texture_id;  // ID handle of the glyph texture
	glm::ivec2 size;       // Size of glyph
	glm::ivec2 bearing;    // Offset from baseline to left/top of glyph
	uint32_t     advance;    // Offset to advance to next glyph
};

// TODO: Can this be refactored into a namespace and used staitically? 
class Renderer
{
public:
	Renderer();
	~Renderer();

	void render(ShaderLibrary shaderLibrary, ShaderLibrary::SHADER_TYPE type, const std::shared_ptr<GameObject> object, glm::mat4 transformation);
	void render_text(ShaderLibrary shaderLibrary, ShaderLibrary::SHADER_TYPE type, std::string text, float x, float y, float scale, glm::vec4 colour);
private:
	std::map<char, character> m_characters;
	FT_Library m_ft;
	uint32_t m_VAO, m_VBO;
};
