#include <utility>
#include <vector>

struct Angle
{
	double a, b;
	bool visited = false;

	Angle() = default;
	Angle(double A, double B) :
		a(A), b(B)
	{
		if (b < a)
			std::swap(a, b);
	}
}; 

using AngleVector = std::vector<Angle>;

class AngleSet
{
public:
	AngleSet(const AngleVector& Set) :
		_set(Set)
	{

	}
	~AngleSet() = default;

	Angle Intersect(const Angle &Angle)
	{

	}

	void Intersect(const AngleVector& Set)
	{

	}

	void Unite(const AngleVector& Set)
	{

	}

private:
	AngleVector _set;
};
