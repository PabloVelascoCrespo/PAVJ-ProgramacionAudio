
#include "Vec2.h"
#include <iostream>
#include <math.h>
#include <stdexcept>

const float PI = 3.14159265359;
Vec2::Vec2() : x(0), y(0) {}

Vec2::Vec2(float _x, float _y) : x(_x), y(_y) {}

Vec2::~Vec2() = default;

float Vec2::getX() const
{
	return x;
}

void Vec2::setX(float _x)
{
	x = _x;
}

float Vec2::getY() const
{
	return y;
}

void Vec2::setY(float _y)
{
	y = _y;
}

Vec2 Vec2::operator+(const Vec2& _other) const
{
	return Vec2(x + _other.getX(), y + _other.getY());
}

Vec2 Vec2::operator+(const float _scalar) const
{
	return Vec2(x + _scalar, y + _scalar);
}

Vec2 Vec2::operator-(const Vec2& _other) const
{
	return Vec2(x - _other.getX(), y - _other.getY());
}

Vec2 Vec2::operator-(const float _scalar) const
{
	return Vec2(x - _scalar, y - _scalar);
}

Vec2 Vec2::operator*(const Vec2& _other) const
{
	return Vec2(x * _other.getX(), y * _other.getY());
}

Vec2 Vec2::operator*(const float _scalar) const
{
	return Vec2(x * _scalar, y * _scalar);
}

float Vec2::dot(const Vec2& _other) const
{
	return x * _other.getX() + y * _other.getY();
}

float Vec2::cross(const Vec2& _other) const
{
	return (x * _other.getY() - y * _other.getX());
}

Vec2 Vec2::operator/(const Vec2& _other) const
{
	return Vec2(x / _other.getX(), y / _other.getY());
}

Vec2 Vec2::operator/(const float _scalar) const
{
	return Vec2(x / _scalar, y / _scalar);
}

bool Vec2::operator==(const Vec2& _other) const
{
	if ((x == _other.getX()) && (y == _other.getY()))
	{
		return true;
	}
	return false;
}

bool Vec2::operator!=(const Vec2& _other) const
{
	return !(*this == _other);
}


Vec2 Vec2::absolute() const
{
	return Vec2(abs(x), abs(y));
}

float Vec2::length() const
{
	return sqrt((x * x) + (y * y));
}

Vec2 Vec2::normalize() const
{
	float length = this->length();
	return length == 0 ? Vec2() : *this / length;
}

float Vec2::angle_between_vec(const Vec2& _other) const
{
	this->print();
	_other.print();

	float dotProduct = this->dot(_other);

	float magnitudesProduct = this->length() * _other.length();

	if (magnitudesProduct == 0)
	{
		throw std::invalid_argument("Uno de los vectores tiene magnitud cero.");
	}

	return acos(dotProduct / magnitudesProduct) * 180.f / PI;
}
float Vec2::angle_between_points(const Vec2& _other) const
{
	return atan2(y - _other.getY(), x - _other.getX()) * 180.f / PI;
}

float Vec2::distance(const Vec2& _other) const
{
	Vec2 dist = *this - _other;
	return dist.length();
}

void Vec2::print() const
{
	std::cout << "(" << x << ", " << y << ")" << std::endl;
}

void Vec2::orbit(const Vec2 _centre, const float _degrees, const float _radius, const double _elapsed_seconds, double& _current_angle)
{
	_current_angle += _degrees * _elapsed_seconds;

	x = _centre.getX() + _radius * cos(_current_angle);
	y = _centre.getY() + _radius * sin(_current_angle);
}
