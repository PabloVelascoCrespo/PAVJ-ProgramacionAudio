#pragma once
class Vec2
{
private:
	float x;
	float y;
public:
	Vec2();
	Vec2(float _x, float _y);
	~Vec2();

	float getX() const;
	float getY() const;
	Vec2 operator+(const Vec2& _other) const;
	Vec2 operator+(const float _scalar) const;
	Vec2 operator-(const Vec2& _other) const;
	Vec2 operator-(const float _scalar) const;
	Vec2 operator*(const Vec2& _other) const;
	Vec2 operator*(const float _scalar) const;
	float dot(const Vec2& _other) const;
	float cross(const Vec2& _other) const;
	Vec2 operator/(const Vec2& _other) const;
	Vec2 operator/(const float _scalar) const;
	bool operator==(const Vec2& _other) const;
	bool operator!=(const Vec2& _other) const;
	Vec2 absolute() const;
	float length() const;
	Vec2 normalize() const;
	float angle_between_vec(const Vec2& _other) const;
	float angle_between_points(const Vec2& _other) const;
	float distance(const Vec2& _other) const;
	void print() const;
	void orbit(const Vec2 _centre, const float _degrees, const float _radius, const double _elapsed_seconds, double& _current_angle);


};

