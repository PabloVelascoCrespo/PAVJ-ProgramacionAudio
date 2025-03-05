#pragma once
#include "Collider.h"
class RectCollider : public Collider
{
public:				// ES TEMPORAL DESPUÉS HAY QUE PASARLO A PRIVATE
	Vec2 position;
	Vec2 size;

public:
	RectCollider(Vec2& _rectPos, Vec2& _rectSize);
	~RectCollider() = default;

	virtual bool collides(const Collider& _other) const override;
	virtual bool collides(const Vec2& _circlePos, const float _circleRadius) const override;
	virtual bool collides(const Vec2& _rectPos, const Vec2& _rectSize) const override;
	virtual bool collides(const Vec2& _pixelsPos, const Vec2& _pixelsSize, const uint8_t* _pixels) const override;
	virtual void setPosition(const Vec2& newPosition) override;
	virtual void setSize(const Vec2& _newSize) override;

};

