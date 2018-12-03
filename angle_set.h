#include <utility>
#include <vector>
#include <algorithm>

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
    SortAndUnite();
  }
  AngleSet() = default;

  ~AngleSet() = default;

  bool Intersected(const Angle &Ang, AngleSet &Res, std::vector <double> &realAngles)
  {
    Res._set.clear();
    realAngles.clear();

    for (int i = 0; i < _set.size();)
    {
      Angle &cur = _set[i];
      if (cur.a > Ang.b)
        return Res._set.size() > 0;
      if (cur.b < Ang.a)
      {
        i++;
        continue;
      }

      Res._set.push_back(cur);
      /// Find intersection to get visual angle
      double realAngle;
      if (Ang.a < cur.a)
        if (Ang.b < cur.b)
          realAngle = (cur.a + Ang.b) / 2;
        else
          realAngle = (cur.a + cur.b) / 2;
      else
        if (Ang.b < cur.b)
          realAngle = (Ang.a + Ang.b) / 2;
        else
          realAngle = (Ang.a + cur.b) / 2;
      realAngles.push_back(realAngle);
      _set.erase(_set.begin() + i);
    }
    return Res._set.size() > 0;
  }

  AngleSet Intersect(const AngleSet& Set)
  {
    AngleVector res;
    const AngleVector &oSet = Set._set;

    for (const Angle &a1 : _set)
      for (const Angle &a2 : Set._set)
      {
        if (a1.a > a2.b)
          break;
        if (a2.a > a1.b)
          continue;
        /// Find intersection
        if (a1.a < a2.a)
          if (a1.b < a2.b)
            res.push_back(Angle(a2.a, a1.b));
          else
            res.push_back(Angle(a2.a, a2.b));
        else
          if (a1.b < a2.b)
            res.push_back(Angle(a1.a, a1.b));
          else
            res.push_back(Angle(a1.a, a2.b));
      }
    return AngleSet(res);
  }

  AngleSet Unite(const AngleSet& Set)
  {
    AngleVector res;

    res = _set;
    res.insert(res.end(), Set._set.begin(), Set._set.end());
    return AngleSet(res);
  }

  AngleVector GetAngleSetVec( void )
  {
    return _set;
  }

private:

  void SortAndUnite()
  {
    auto sortF = [](const Angle &a1, const Angle &a2) { return a1.a <= a2.a; };
    std::sort(_set.begin(), _set.end(), sortF);

    int i = 0;
    for (; i < _set.size() - 1;)
    {
      Angle &cur = _set[i];
      Angle &next = _set[i + 1];
      if (next.a > cur.b)
      {
        i++;
        continue;
      }
      cur.b = next.b;
      _set.erase(_set.begin() + i + 1);
    }
  }

  /// Class variables
  AngleVector _set;
};
