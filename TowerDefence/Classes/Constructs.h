#ifndef CONSTRUCTS_H
#define CONSTRUCTS_H

#include <iostream>

class Point2i
{
public:
	int x,  y;

	Point2i(const int &X, const int &Y);
	Point2i();
	
	bool operator==(Point2i &other) const;
};

class Vec2f 
{
public:
	float x, y;

	Vec2f();
	Vec2f(const float &X, const float &Y);
		
	Vec2f operator*(const float &scale) const;
	Vec2f operator/(const float &scale) const;
	Vec2f operator+(const Vec2f &other) const;
	Vec2f operator-(const Vec2f &other) const;
	Vec2f operator-() const;
		
	const Vec2f &operator*=(const float &scale);
	const Vec2f &operator/=(const float &scale);
	const Vec2f &operator+=(const Vec2f &other);
	const Vec2f &operator-=(const Vec2f &other);
		
	float magnitude() const;
	float magnitudeSquared() const;
	Vec2f normalize() const;
	float dot(const Vec2f &other) const;
	float cross(const Vec2f &other) const;
};

Vec2f operator*(const float &scale, const Vec2f &v);
std::ostream &operator<<(std::ostream &output, const Vec2f &v);

#endif