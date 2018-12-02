#pragma once

class Geometry {
public:
	virtual void Rotate(double angle, const Vector2d& point) = 0;
	virtual void MoveParallel(const Vector2d& moveVec) = 0;

	
};