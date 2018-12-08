#include <utility>
#include <vector>
#include <algorithm>
#include <math.h>

#include <assert.h>

const static double PI2 = M_PI * 2;


class Angle
{
  using AngleVector = std::vector<Angle>;

public:
  double start, delta;
  bool isEmpty, isFull;
  bool isGood;

  Angle() :
    isEmpty(true),
    isFull(false),
    isGood(true)
  {
  };

  Angle(double Start, double Finish) :
    start(Start), delta(abs(Finish - Start)),
    isEmpty(false), isFull(false)
  {
    if (delta >= PI2)
    {
      delta = PI2;
      isFull = true;
      start = 0;
      return;
    }
    if (Start > Finish)
    {
      delta = PI2 - delta;
    }

    while (start < 0)
      start += PI2;
    while (start > PI2)
      start -= PI2;
  }

  /*
  Angle IntersectGood(double a, double b, double c, double d)
  {
    if (a > d)
      return Angle();
    if (c > b)
      return Angle();
    /// Find intersection
    if (a < c)
      if (b < d)
        return Angle(c, b);
      else
        return Angle(c, d);
    else
      if (b < d)
        return Angle(a, b);
      else
        return Angle(a, d);
  }

  AngleVector UniteGood(double a, double b, double c, double d)
  {
    AngleVector res;
    if (!IntersectGood(a, b, c, d).isEmpty){
      res.push_back(Angle(min(a, c), max(b, d)));
      return res;
    }
    else
    {
      res.push_back(Angle(a, b));
      res.push_back(Angle(c, d));
      return res;
    }
  }

  AngleVector Unite( const Angle &An)
  {
    AngleVector res;

    if (isEmpty && An.isEmpty)
    {
      return res;
    }
    else if (isEmpty)
    {
      return res.push_back(An);
    }
    else if (An.isEmpty)
    {
      return res.push_back(*this);
    }
     
    double a = start, b = start + delta;
    double c = An.start, d = An.start + delta;

    if ((b <= PI2 && d <= PI2) || (b > PI2 && d > PI2)) {
      return UniteGood(a, b, c, d);
    }

    const Angle &norm = *this;
    const Angle &wrong = An;
    if (b > PI2)

      std::swap(norm, wrong);

    Angle part1 = Angle(wrong.start, PI2);
    Angle part2 = Angle(0, wrong.start + delta - PI2);

    if (norm.start > wrong.start + wrong.delta - PI2) {
      res.push_back(Angle(wrong.start, norm.start + norm.delta));
    }
    else if (norm.start + norm.delta <= wrong.start) {
      res.push_back(Angle(norm.start, wrong.start + wrong.delta - PI2));
    }
    else {
      res.push_back(Angle(wrong.start, norm.start + norm.delta));
      res.push_back(Angle(norm.start, wrong.start + wrong.delta - PI2));
    }
    return res;

  }

  AngleVector GetSimples(void) const
  {
    AngleVector res;
    if (start + delta <= PI2) {
      res.push_back(*this);
    }
    else {
      res.push_back(Angle(start, PI2));
      res.push_back(Angle(0, start + delta - PI2));
    }
    return res;
  }
*/


  static AngleVector IntersectGoodWithGood(const Angle &fst, const Angle &scnd)
  {
    assert(fst.isGood && scnd.isGood);
    AngleVector res;
    double a = fst.start;
    double b = fst.start + fst.delta;
    double c = scnd.start;
    double d = scnd.start + scnd.delta;

    if (a <= c && d <= b) {
      res.push_back(Angle(c, d));
    }
    else if (c <= a && b <= d) {
      res.push_back(Angle(a, b));
    }
    else if (c >= a && d >= b && b >= c) {
      res.push_back(Angle(c, b));
    }
    else if (c <= a && d <= b && d >= a) {
      res.push_back(Angle(a, d));
    }
    
    return res;
  }  

  static AngleVector IntersectBadWithGood(const Angle &bad, const Angle &good)
  {
    assert(!(bad.isGood) && good.isGood);

    double a = good.start;
    double b = good.start + good.delta;
    double c = bad.start;
    double d = bad.start + bad.delta;
    AngleVector res;

         if (c <= b && d >= a) {
      res.push_back(Angle(a, d));
      res.push_back(Angle(c, b));
    }

    else if (c <= b && c >= a && d <= a) {
      res.push_back(Angle(c, b));
    }

    else if (b <= c && a <= d && d <= b) {
      res.push_back(Angle(a, d));
    }

    else if ((d >= b && c >= b) || (d <= a && c <= a)) {
      res.push_back(Angle(a, b));
    }

    else {
      assert("Bad with good intersection failed");
    }
    return res;
  }

  static AngleVector IntersectBadWithBad(const Angle &fst, const Angle &scnd)
  {
    assert(!(fst.isGood) && !(!scnd.isGood));

    double a = fst.start;
    double b = fst.start + fst.delta;
    double c = scnd.start;
    double d = scnd.start + scnd.delta;
    AngleVector res;

    if (d >= a && c >= a) {
      res.push_back(Angle(c, b));
      res.push_back(Angle(a, d));
    }

    else if (d <= b && c <= b) {
      res.push_back(Angle(c, b));
      res.push_back(Angle(a, d));
    }

    else if (d <= b && c >= a) {
      res.push_back(scnd);      
    }

    else if (d >= b && c >= a && d <= a) {
      res.push_back(Angle(c, b));
    }

    else if (d <= b && c <= a && c >= b) {
      res.push_back(Angle(a, d));
    }

    else if (b <= d && c <= a) {
      res.push_back(Angle(a, b));
    }

    else {
      assert("Bad with Bad Failed");
    }
    return res;
  }

  AngleVector Intersect( const Angle &An )
  {
    if (isGood) {
      if (An.isGood) {
        return Angle::IntersectGoodWithGood(*this, An);
      }
      else {
        return Angle::IntersectBadWithGood(An, *this);
      }
    } 
    else {
      if (An.isGood) {
        return Angle::IntersectBadWithGood(*this, An);
      }
      else {
        return Angle::IntersectBadWithBad(An, *this);
      }
    } 
  }

  static AngleVector UnionGoodWithGood(const Angle &fst, const Angle &scnd)
  {
    assert(fst.isGood && scnd.isGood);
    AngleVector res;
    double a = fst.start;
    double b = fst.start + fst.delta;
    double c = scnd.start;
    double d = scnd.start + scnd.delta;

    if (a <= c && d <= b) {
      res.push_back(Angle(a, b));
    }
    else if (c <= a && b <= d) {
      res.push_back(Angle(c, d));
    }
    else if (c >= a && d >= b && b >= c) {
      res.push_back(Angle(a, d));
    }
    else if (c <= a && d <= b && d >= a) {
      res.push_back(Angle(c, b));
    }
    else if (b <= c || a >= d) { 
      res.push_back(Angle(a, b));
      res.push_back(Angle(c, d));
    }

    return res;
  }

  static AngleVector UnionBadWithGood(const Angle &bad, const Angle &good)
  {
    assert(!(bad.isGood) && good.isGood);

    double a = good.start;
    double b = good.start + good.delta;
    double c = bad.start;
    double d = bad.start + bad.delta;
    AngleVector res;

    if (c <= b && d >= a) {
      res.push_back(Angle(0, PI2));
    }

    else if (c <= b && c >= a && d <= a) {
      res.push_back(Angle(a, d));
    }

    else if (b <= c && a <= d && d <= b) {
      res.push_back(Angle(c, b));
    }

    else if ((d >= b && c >= b) || (d <= a && c <= a)) {
      res.push_back(Angle(c, d));
    }

    else {
      assert("Bad with good intersection failed");
    }
    return res;
  }

  static AngleVector UnionBadWithBad(const Angle &fst, const Angle &scnd)
  {
    assert(!(fst.isGood) && !(!scnd.isGood));

    double a = fst.start;
    double b = fst.start + fst.delta;
    double c = scnd.start;
    double d = scnd.start + scnd.delta;
    AngleVector res;

    if (d >= a && c >= a) {
      res.push_back(Angle(0, PI2));
    }

    else if (d <= b && c <= b) {
      res.push_back(Angle(0, PI2));
    }

    else if (d <= b && c >= a) {
      res.push_back(Angle(a, b));
    }

    else if (d >= b && c >= a && d <= a) {
      res.push_back(Angle(a, d));
    }

    else if (d <= b && c <= a && c >= b) {
      res.push_back(Angle(c, b));
    }

    else if (b <= d && c <= a) {
      res.push_back(Angle(c, d));
    }

    else {
      assert("Bad with Bad Failed");
    }
    return res;
  }


  AngleVector Union(const Angle &An)
  {
    if (isGood) {
      if (An.isGood) {
        return Angle::UnionGoodWithGood(*this, An);
      }
      else {
        return Angle::UnionBadWithGood(An, *this);
      }
    }
    else {
      if (An.isGood) {
        return Angle::UnionBadWithGood(*this, An);
      }
      else {
        return Angle::UnionBadWithBad(An, *this);
      }
    }
  }

  Angle Ñomplement()
  {
    if (isFull)
      return Angle();
    if (isEmpty)
      return Angle(0, PI2);
    return Angle(start + delta, start);
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

  void UnionSetWithVector(const AngleVector &AnVec){
    for (const Angle &AngleO : AnVec)
    {
      UnionSelf(AngleO);
    }
  }

  void UnionSelf(const Angle& An){
    AngleVector res;
    AngleVector res1;

    for (Angle& AnglSelf : _set)
    {
      res1 = AnglSelf.Union(An);
      res.insert(res.end(), res1.begin(), res1.end());
    }
    _set = res;
  }

  bool Intersected(const Angle &Ang, AngleSet &Res, std::vector <double> &realAngles)
  {
    Res._set.clear();

    for (int i = 0; i < _set.size();)
    {
      AngleVector inter = _set[i].Intersect(Ang);
      if (inter.size() == 0)
      {
        i++;
        continue;
      }

      Res._set.push_back(_set[i]);
      realAngles.push_back(inter[0].start + inter[0].delta / 2);
      _set.erase(_set.begin() + i);
    }
    return realAngles.size() > 0;
  }

  void IntersectSelf(const Angle &An)
  {
    AngleVector res;
    AngleVector res1;

    for (Angle& AnglSelf : _set)
    {
      res1 = AnglSelf.Intersect(An);
      res.insert(res.end(), res1.begin(), res1.end());
    }
    _set = res;
  }

  void IntersectSetWithVector(const AngleVector &AnVec)
  {
    for (const Angle &AngleO : AnVec) 
    {
      IntersectSelf(AngleO);
    }
  }

  AngleVector & GetAngleSetVec( void )
  {
    return _set;
  }

  bool Empty()
  {
    return _set.size() == 0;
  }

private:
  /// Class variables
  AngleVector _set;
};
