#pragma once

#include "geometry.h"
#include "point.h"


class Segment : public Geometry {
public:
	Segment(const Vector2d& vB, const Vector2d& vE)
		: pBegin(vB)
		, pEnd(vE)
	{
	}

	Segment(double xB, double yB, double xE, double yE) 
		: pBegin(xB, yB)
		, pEnd(xE, yE)
	{
	}

  
	void Rotate(double angle, const Vector2d& point) override
	{
		pBegin = (pBegin - point).Rotate(angle) + point;
		pEnd = (pEnd - point).Rotate(angle) + point;
	}

	void MoveParallel(const Vector2d& moveVec) override {
		pBegin = pBegin + moveVec;
		pEnd = pEnd + moveVec;
	}

  Vector2d GetBegin() const
  {
    return pBegin;
  }

  Vector2d GetEnd() const
  {
    return pEnd;
  }

	
public:
	Vector2d pBegin, pEnd;
};