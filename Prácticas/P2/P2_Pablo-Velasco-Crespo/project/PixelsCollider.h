#pragma once
#include "Collider.h"

class PixelsCollider : public Collider
{
public:
	Vec2 position;
	Vec2 size;
	uint8_t* pixels;

public:
	PixelsCollider(Vec2& _pixelsPos, Vec2& _pixelsSize, uint8_t* _pixels);
	~PixelsCollider();

	virtual bool collides(const Collider& _other) const override;
	virtual bool collides(const Vec2& _circlePos, float _circleRadius) const override;
	virtual bool collides(const Vec2& _rectPos, const Vec2& _rectSize) const override;
	virtual bool collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const override;
	
	virtual void setPosition(const Vec2& newPosition) override;
	virtual void setSize(const Vec2& _newSize) override;
	void setPixels(uint8_t* _newPixels);

};

