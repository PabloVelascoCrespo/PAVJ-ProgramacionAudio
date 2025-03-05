#pragma once
#include <litegfx.h> // NO ME GUSTA TENER LA LIBRERÍA AQUÍ, HAY QUE SOLUCIONARLO
#include "Vec2.h"
#include <math.h>
#include "Collider.h"

class Sprite
{
public:
	enum CollisionType
	{
		COLLISION_NONE,
		COLLISION_CIRCLE,
		COLLISION_RECT,
		COLLISION_PIXELS

	};

private:
	typedef void (*CallbackFunc)(Sprite&, float);

	const ltex_t* texture;
	lblend_t mode;

	float red;
	float green;
	float blue;
	float alpha;

	Vec2 position;

	float angle;
	Vec2 scale;
	Vec2 pivot;

	int hFrames;
	int vFrames;

	int fps;
	int currentFrame;

	CallbackFunc callback;
	void* userData;

	double total_time; // esta variable la uso para ir acumulando el delta time y saber cuándo cambiar de frame por los fps

	Collider* collider;
	CollisionType type;

public:

	Sprite(const ltex_t* _tex, int _hframes = 1, int _vframes = 1);
	Sprite();
	~Sprite();

	void setCallback(CallbackFunc _func);

	void setUserData(void* _data);
	void* getUserData() const;

	const ltex_t* getTexture() const;
	void setTexture(const ltex_t* _tex, int _hframes = 1, int _vframes = 1);

	lblend_t getBlend() const;
	void setBlend(lblend_t _mode);

	float getRed() const;
	float getGreen() const;
	float getBlue() const;
	float getAlpha() const;
	void setColor(float _r, float _g, float _b, float _a);

	const Vec2& getPosition() const;
	void setPosition(const Vec2& _pos);

	float getAngle() const;
	void setAngle(float _angle);

	const Vec2& getScale() const;
	void setScale(const Vec2& _scale);
	Vec2 getSize() const;

	const Vec2& getPivot() const;
	void setPivot(const Vec2& _pivot);

	int getHframes() const;
	int getVframes() const;

	int getFps() const;
	void setFps(int _fps);
	float getCurrentFrame() const;
	void setCurrentFrame(int _frame);

	void setCollisionType(CollisionType _type);
	Sprite::CollisionType getCollisionType() const;
	const Collider* getCollider() const;
	bool collides(const Sprite& _other) const;

	void update(float _deltaTime);
	void draw() const;

};