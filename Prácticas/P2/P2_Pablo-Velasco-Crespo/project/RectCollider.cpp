#include "RectCollider.h"

RectCollider::RectCollider(Vec2& _rectPos, Vec2& _rectSize)
	: position(_rectPos)
	, size(_rectSize)
{
}

bool RectCollider::collides(const Collider& _other) const
{
	return _other.collides(position, size);
}

bool RectCollider::collides(const Vec2& _circlePos, float _circleRadius) const
{
	return checkCircleRect(_circlePos, _circleRadius, position, size);
}

bool RectCollider::collides(const Vec2& _rectPos, const Vec2& _rectSize) const
{
	return checkRectRect(position, size, _rectPos, _rectSize);
}

bool RectCollider::collides(const Vec2& _pixelsPos, const Vec2& _pixelsSize, const uint8_t* _pixels) const
{
	return checkPixelsRect(_pixelsPos, _pixelsSize, _pixels, position, size);
}

void RectCollider::setPosition(const Vec2& newPosition)
{
	position = newPosition;
}

void RectCollider::setSize(const Vec2& _newSize)
{
	size = _newSize;
}
