#include "Font.h"
#include <stdio.h>
#include <glfw3.h>
#include <litegfx.h>
#include <utility>

Font::~Font()
{
	if (alphaBuffer) {
		delete[] alphaBuffer;
		alphaBuffer = nullptr;
	}

	if (textureID) {
		glDeleteTextures(1, &textureID);
		textureID = 0;
	}
}

Font* Font::load(const char* filename, float height)
{
	Font* font = new Font(height);
	if (!font->loadFontData(filename))
	{
		delete font;
		return nullptr;
	}
	font->generateTexture();
	return font;
}

float Font::getHeight() const
{
	return height;
}

Vec2 Font::getTextSize(const char* text) const
{
	float width = 0.0f, maxHeight = 0.0f;
	float x = 0.0f, y = 0.0f;

	for (const char* p = text; *p; ++p)
	{
		stbtt_aligned_quad q;
		stbtt_GetBakedQuad(bakeData, 512, 512, *p - 32, &x, &y, &q, 1);
		width = q.x1; // Ancho total
		maxHeight = std::max(maxHeight, q.y1 - q.y0); // Máximo alto
	}

	return Vec2(width, maxHeight);
}

void Font::draw(const char* text, const Vec2& pos) const
{
	glBindTexture(GL_TEXTURE_2D, textureID);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float x = pos.getX(), y = pos.getY();

	for (const char* p = text; *p; ++p)
	{
		stbtt_aligned_quad q;
		stbtt_GetBakedQuad(bakeData, 512, 512, *p - 32, &x, &y, &q, 1);

		glBegin(GL_QUADS);
		glTexCoord2f(q.s0, q.t0); glVertex2f(q.x0, q.y0);
		glTexCoord2f(q.s1, q.t0); glVertex2f(q.x1, q.y0);
		glTexCoord2f(q.s1, q.t1); glVertex2f(q.x1, q.y1);
		glTexCoord2f(q.s0, q.t1); glVertex2f(q.x0, q.y1);
		glEnd();
	}

	glDisable(GL_TEXTURE_2D);
}


bool Font::loadFontData(const char* filename)
{
	FILE* fontFile = nullptr;
	errno_t err = fopen_s(&fontFile, filename, "rb");
	if (err != 0 || !fontFile) return false;
	
	fseek(fontFile, 0, SEEK_END);
	size_t fileSize = ftell(fontFile);
	fseek(fontFile, 0, SEEK_SET);

	unsigned char* fontBuffer = new unsigned char[fileSize];

	if (!fontBuffer)
	{
		fclose(fontFile);
		return false;
	}

	fread(fontBuffer, 1, fileSize, fontFile);
	fclose(fontFile);

	stbtt_fontinfo fontInfo;
	if (!stbtt_InitFont(&fontInfo, fontBuffer, stbtt_GetFontOffsetForIndex(fontBuffer, 0)))
	{
		delete[] fontBuffer;
		return false;
	}

	int texWidth = 512, texHeight = 512;
	alphaBuffer = new unsigned char[texWidth * texHeight];
	if (!alphaBuffer)
	{
		delete[] fontBuffer;
		return false;
	}

	if (!stbtt_BakeFontBitmap(fontBuffer, 0, height, alphaBuffer, texWidth, texHeight, 32, 96, bakeData))
	{
		delete[] fontBuffer;
		delete[] alphaBuffer;
		alphaBuffer = nullptr;
		return false;
	}

	delete[] fontBuffer;
	return true;
}
void Font::generateTexture()
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, alphaBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Libera el buffer después de crear la textura
	delete[] alphaBuffer;
	alphaBuffer = nullptr;
}
