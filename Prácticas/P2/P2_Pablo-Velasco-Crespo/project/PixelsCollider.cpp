#include "PixelsCollider.h"
#include <glfw3.h>

PixelsCollider::PixelsCollider(Vec2& _pixelsPos, Vec2& _pixelsSize, uint8_t* _pixels)
	: position(_pixelsPos)
	, size(_pixelsSize)
	, pixels(_pixels)
{
}

PixelsCollider::~PixelsCollider()
{
	delete[] pixels;
}

bool PixelsCollider::collides(const Collider& _other) const
{
	return _other.collides(position, size, pixels);
}

bool PixelsCollider::collides(const Vec2& _circlePos, float _circleRadius) const
{
	return checkCirclePixels(_circlePos, _circleRadius, position, size, pixels);
}

bool PixelsCollider::collides(const Vec2& _rectPos, const Vec2& _rectSize) const
{
	return checkPixelsRect(position, size, pixels, _rectPos, _rectSize);
}

bool PixelsCollider::collides(const Vec2& _pixelsPos, const Vec2& _pixelsSize, const uint8_t* _pixels) const
{
	return checkPixelsPixels(position, size, pixels, _pixelsPos, _pixelsSize, _pixels);
}

void PixelsCollider::setPosition(const Vec2& newPosition)
{
	position = newPosition;
}

void PixelsCollider::setSize(const Vec2& _newSize)
{
	size = _newSize;
}

void PixelsCollider::setPixels(uint8_t* _newPixels)
{
	delete[] pixels;
	pixels = _newPixels;
}
