#pragma once

#include <math.h>

class Vector2d;

class Vector2d {
public:
  Vector2d() {}

	Vector2d(double x_, double y_) {
		x = x_, y = y_;
	}

  bool operator==(Vector2d p) const
  {
    if (fabs(x - p.GetX()) > eps || fabs(y - p.GetY()) > eps)
      return false;
    return true;
  }

  Vector2d operator =(const Vector2d& vec) {
    return Vector2d(x = vec.GetX(), y = vec.GetY());
  }

	void SetX(double x_) {
		x = x_;
	}

	void SetY(double y_) {
		y = y_;
	}

	double GetX(void) const {
		return x;
	}

	double GetY(void) const {
		return y;
	}

	Vector2d operator +(const Vector2d& vec) const {
		return Vector2d(x + vec.GetX(), y + vec.GetY());
	}

	Vector2d operator -(const Vector2d& vec) const {
		return Vector2d(x - vec.GetX(), y - vec.GetY());
	}

	Vector2d operator *(double a) const {
		return Vector2d(x * a, y * a);
	}



	double Lenght(void) const {
		return sqrt(x * x + y * y);
	}

	double AngleBetween(const Vector2d& vec) {
		return acos((x * vec.GetX() + y * vec.GetY()) / Lenght() / vec.Lenght());
	}

	Vector2d Rotate(double angle) {
		return Vector2d(x * cos(angle) - y * sin(angle), x * sin(angle) + y * sin(angle));
	}

	Vector2d Normalise(void) {
		return *this * (1 / Lenght());
	}
private:
  const double eps = 1e-10f;
	double x, y;
};

extern const Vector2d unitX;
extern const Vector2d unitY;
