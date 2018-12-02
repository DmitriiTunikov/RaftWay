#include <utility>
#include <vector>

class Angle
{
	friend class AngleSet;
private:
	double a, b;
public:
	Angle() = default;
	Angle(double A, double B) :
		a(A), b(B)
	{
		if (b < a)
			std::swap(a, b);
	}

	bool IsIn(double AngleValue)
	{
		return AngleValue >= a && AngleValue <= b;
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
	AngleSet() = default;

	~AngleSet() = default;

	bool Intersected(const Angle &Ang, AngleSet &Res)
	{
		
	}

	AngleSet Intersect(const AngleSet& Set)
	{
		AngleSet res;

		for (const Angle &s1 : _set)
			for (const Angle &s2 : Set._set)
			{
				if (s1.a > s2.b || s1.b < s2.a)
					continue;

				if (s2.a <= s2.a)
					;
			}
	}

	AngleSet Unite(const AngleSet& Set)
	{

	}

private:
	AngleVector _set;
};
