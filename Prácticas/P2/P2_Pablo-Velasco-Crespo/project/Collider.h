#pragma once
#include "Vec2.h"

#include <glfw3.h>
class Collider
{
public:
	virtual ~Collider() = default;
	virtual bool collides(const Collider& other) const = 0;
	virtual bool collides(const Vec2& circlePos, const float circleRadius) const = 0;
	virtual bool collides(const Vec2& rectPos, const Vec2& rectSize) const = 0;
	virtual bool collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const = 0;
	virtual void setPosition(const Vec2& newPosition) = 0;
	virtual void setSize(const Vec2& _newSize) = 0;

protected:
	bool checkCircleCircle(const Vec2& pos1, float radius1, const Vec2& pos2, float radius2) const;
	bool checkCircleRect(const Vec2& circlePos, float circleRadius, const Vec2& rectPos, const Vec2& rectSize) const;
	bool checkRectRect(const Vec2& rectPos1, const Vec2& rectSize1, const Vec2& rectPos2, const Vec2& rectSize2) const;
	bool checkCirclePixels(const Vec2& circlePos, float circleRadius, const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const;
	bool checkPixelsPixels(const Vec2& pixelsPos1, const Vec2& pixelsSize1, const uint8_t* pixels1, const Vec2& pixelsPos2, const Vec2& pixelsSize2, const uint8_t* pixels2) const;
	bool checkPixelsRect(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels, const Vec2& rectPos, const Vec2& rectSize) const;
};