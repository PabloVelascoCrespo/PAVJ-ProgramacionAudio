#pragma once
#include "Vec2.h"
#include "stb_truetype.h"

class Font
{
private:
	float height;
	unsigned char* alphaBuffer;
	unsigned int textureID;
	stbtt_bakedchar bakeData[96];

public:
	Font(float height) : height(height), alphaBuffer(nullptr), textureID(0), bakeData() {}
	~Font();

	static Font* load(const char* filename, float height);
	float getHeight() const;
	Vec2 getTextSize(const char * text) const;
	void draw(const char* text, const Vec2& pos) const;

private:
	bool loadFontData(const char* filename);
	void generateTexture();
};

