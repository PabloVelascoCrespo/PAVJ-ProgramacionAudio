#pragma once
#include "Collider.h"
class CircleCollider : public Collider
{
public: // temporal después es private
	Vec2 position;
	float radius;

public:
	CircleCollider(Vec2& _circlePos, float _circleRadius);

	virtual bool collides(const Collider& _other) const override;
	virtual bool collides(const Vec2& _circlePos, float _circleRadius) const override;
	virtual bool collides(const Vec2& _rectPos, const Vec2& _rectSize) const override;
	virtual bool collides(const Vec2& _pixelsPos, const Vec2& _pixelsSize, const uint8_t* _pixels) const override;
	virtual void setPosition(const Vec2& newPosition) override;
	virtual void setSize(const Vec2& _newRadius) override;
};

