#ifndef __TEXTRENDERER_H
#define __TEXTRENDERER_H

#include <engine\shader.hpp>
#include <map>
#include <glad\glad.h>
#include <vector>


/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	uint32_t TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
	uint32_t Advance;     // Horizontal offset to advance to next glyph
};
struct TextQueue {
	std::string TextQueue_text;
	float TextQueue_x;
	float TextQueue_y;
	float TextQueue_scale;
	glm::vec3 TextQueue_color;
};

// A renderer class for rendering text displayed by a font loaded using the 
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.
class TextRenderer
{
public:

	

	// Holds a list of pre-compiled Characters
	std::map<GLchar, Character> Characters;
	// Shader used for text rendering
	const Shader& _textShader;

	// Constructor
	TextRenderer(const Shader& _textShader, uint32_t width, uint32_t height);
	//TextRenderer() = delete;
	~TextRenderer();

	// Pre-compiles a list of characters from the given font
	void Load(std::string font, uint32_t fontSize);
	// Renders a string of text using the precompiled list of characters
	void RenderText();

	void addTextToRender(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));

private:
	// Render state
	uint32_t _width;
	uint32_t _height;
	uint32_t VAO, VBO;
	std::vector<TextQueue> _renderQueue;

};

#endif