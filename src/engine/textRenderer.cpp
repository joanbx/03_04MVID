#include <engine\textRenderer.hpp>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>


#include FT_FREETYPE_H

TextRenderer::TextRenderer(const Shader& textShader, uint32_t width, uint32_t height) : _textShader(textShader), _width(width), _height(height)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

TextRenderer::~TextRenderer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void TextRenderer::Load(std::string font, uint32_t fontSize)
{
	
	// First clear the previously loaded Characters
	Characters.clear();
	// Then initialize and load the FreeType library
	FT_Library ft;


	if (FT_Init_FreeType(&ft)) // All functions return a value different than 0 whenever an error occurred
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, font.c_str(), 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, fontSize);
	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Then for the first 128 ASCII characters, pre-load/compile their characters and store them
	for (GLubyte c = 0; c < 128; c++) // lol see what I did there 
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
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
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRenderer::RenderText()
{
	// Activate corresponding render state	
	
	_textShader.use();
	_textShader.set("projection", glm::ortho(0.0f, static_cast<float>(_width), static_cast<float>(_height), 0.0f));

	//std::cout << "RENDER TEXT: " << color.x  << std::endl;
	// Iterate through all characters
	for (auto& _rq : _renderQueue) {
		_textShader.set("text", 0);
		_textShader.set("textColor", _rq.TextQueue_color);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);
		std::string::const_iterator c;
		for (c = _rq.TextQueue_text.begin(); c != _rq.TextQueue_text.end(); c++)
		{
			Character ch = Characters[*c];

			float xpos = _rq.TextQueue_x + ch.Bearing.x * _rq.TextQueue_scale;
			float ypos = _rq.TextQueue_y + (Characters['H'].Bearing.y - ch.Bearing.y) * _rq.TextQueue_scale;

			float w = ch.Size.x * _rq.TextQueue_scale;
			float h = ch.Size.y * _rq.TextQueue_scale;
			// Update VBO for each character
			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0, 1.0 },
				{ xpos + w, ypos,       1.0, 0.0 },
				{ xpos,     ypos,       0.0, 0.0 },

				{ xpos,     ypos + h,   0.0, 1.0 },
				{ xpos + w, ypos + h,   1.0, 1.0 },
				{ xpos + w, ypos,       1.0, 0.0 }
			};
			// Render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			// Update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// Now advance cursors for next glyph
			_rq.TextQueue_x += (ch.Advance >> 6)* _rq.TextQueue_scale; // Bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	_renderQueue.clear();

	
}

void TextRenderer::addTextToRender(std::string text, float x, float y, float scale, glm::vec3 color) {

	TextQueue newTextQueue;
	newTextQueue.TextQueue_text = text;
	newTextQueue.TextQueue_x = x;
	newTextQueue.TextQueue_y = y;
	newTextQueue.TextQueue_scale = scale;
	newTextQueue.TextQueue_color = color;

	_renderQueue.push_back(newTextQueue);
}