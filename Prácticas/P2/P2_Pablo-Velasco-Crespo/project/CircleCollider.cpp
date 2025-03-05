#include "CircleCollider.h"

CircleCollider::CircleCollider(Vec2& _circlePos, float _circleRadius)
	: position(_circlePos)
	, radius(_circleRadius)
{
}

bool CircleCollider::collides(const Collider& _other) const
{
	return _other.collides(position, radius);
}

bool CircleCollider::collides(const Vec2& _circlePos, float _circleRadius) const
{
	return checkCircleCircle(position, radius, _circlePos, _circleRadius);
}

bool CircleCollider::collides(const Vec2& _rectPos, const Vec2& _rectSize) const
{
	return checkCircleRect(position, radius, _rectPos, _rectSize);
}

bool CircleCollider::collides(const Vec2& _pixelsPos, const Vec2& _pixelsSize, const uint8_t* _pixels) const
{
	return checkCirclePixels(position, radius, _pixelsPos, _pixelsSize, _pixels);
}

void CircleCollider::setPosition(const Vec2& newPosition)
{
	position = newPosition;
}

void CircleCollider::setSize(const Vec2& _newRadius)
{
	radius = _newRadius.getX();
}
