#pragma once

#include "geometry.h"
#include "point.h"

class HalfCircle : public Geometry {
public:
	HalfCircle(const Vector2d& center_, double radius_, const Vector2d& circleFront_)
		: center(center_)
		, radius(radius_)
		, circleFront(circleFront_)
	{
	}

	virtual void Rotate(double angle, const Vector2d& point) override
	{
		circleFront = (circleFront - point).Rotate(angle) + point;
	}

	virtual void MoveParallel(const Vector2d& moveVec) override {
		circleFront = circleFront + moveVec;
	}


private:
	Vector2d circleFront;
	Vector2d center;
	double radius;
};
