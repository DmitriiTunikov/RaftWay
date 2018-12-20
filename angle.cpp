#include "angle.h"
#include "angle_set.h"

using namespace std;

vector<Angle> Angle::getGoods(const Angle& an) {
  vector<Angle> res;

  res.push_back(Angle(an.start, PI2));
  res.push_back(Angle(0, an.start + an.delta - PI2));

  return res;
}

Angle Angle::IntersectGoodWithGood(const Angle &fst, const Angle &scnd)
{
  assert(fst.isGood && scnd.isGood);
  Angle res;
  res.isEmpty = true;
  double a = fst.start;
  double b = fst.start + fst.delta;
  double c = scnd.start;
  double d = scnd.start + scnd.delta;

  if (Less(a, c) && Less(d, b)) {
    res = (Angle(c, d));
  }
  else if (Less(c, a) && Less(b, d)) {
    res = (Angle(a, b));
  }
  else if (More(c, a) && More(d, b) && More(b, c)) {
    res = (Angle(c, b));
  }
  else if (Less(c, a) && Less(d, b) && More(d, a)) {
    res = (Angle(a, d));
  }
  return res;
}

std::vector<Angle> Angle::IntersectBadWithGood(const Angle &bad, const Angle &good)
{
  assert(!(bad.isGood) && good.isGood);

  //get goods angles
  std::vector<Angle> goods1 = getGoods(bad);
  vector<Angle> goods2{ good };

  //intersect good1 with goods2
  return IntersectGoodVecWithGoodVec(goods1, goods2);
}

vector<Angle> Angle::IntersectGoodVecWithGoodVec(const vector<Angle>& goods1, const vector<Angle>& goods2) {
  //intersect good1 with goods2
  vector<Angle> res;
  Angle a, b;
  a.isEmpty = true;
  b.isEmpty = true;
  for (const Angle& an1 : goods1) {
    for (const Angle& an2 : goods2) {
      Angle ang = IntersectGoodWithGood(an1, an2);
      if (ang.isEmpty)
        continue;

      if (Equal(ang.start + ang.delta, PI2))
        a = ang;
      else if (Equal(ang.start, 0)) {
        b = ang;
      }
      else
        res.push_back(ang);
    }
  }
  //push bad joined angles as one
  if (a.isEmpty && b.isEmpty)
    return res;
  if (!a.isEmpty && !b.isEmpty)
    res.push_back(Angle(a.start, b.delta));
  else if (b.isEmpty)
    res.push_back(Angle(a.start, PI2));
  else if (a.isEmpty)
    res.push_back(Angle(0, b.delta));

  return res;
}

std::vector<Angle> Angle::IntersectBadWithBad(const Angle &fst, const Angle &scnd)
{
  assert(!(fst.isGood) && !(scnd.isGood));

  //get goods angles
  vector<Angle> goods1 = getGoods(fst);
  vector<Angle> goods2 = getGoods(scnd);

  //intersect good1 with goods2
  return IntersectGoodVecWithGoodVec(goods1, goods2);

}

std::vector<Angle> Angle::Intersect(const Angle &An)
{
  if (isEmpty || An.isEmpty)
    return std::vector<Angle>();

  if (isGood) {
    if (An.isGood) {
      return vector<Angle>{Angle::IntersectGoodWithGood(*this, An)};
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

std::vector<Angle> Angle::UnionGoodWithGood(const Angle &fst, const Angle &scnd)
{
  assert(fst.isGood && scnd.isGood);
  std::vector<Angle> res;
  double a = fst.start;
  double b = fst.start + fst.delta;
  double c = scnd.start;
  double d = scnd.start + scnd.delta;

  if (Less(a, c) && Less(d, b)) {
    res.push_back(Angle(a, b));
  }
  else if (Less(c, a) && Less(b, d)) {
    res.push_back(Angle(c, d));
  }
  else if (More(c, a) && More(d, b) && More(b, c)) {
    res.push_back(Angle(a, d));
  }
  else if (Less(c, a) && Less(d, b) && More(d, a)) {
    res.push_back(Angle(c, b));
  }
  else if (Less(b, c) || More(a, d)) {
    res.push_back(Angle(a, b));
    res.push_back(Angle(c, d));
  }

  return res;
}

std::vector<Angle> Angle::UnionBadWithGood(const Angle &bad, const Angle &good)
{
  assert(!(bad.isGood) && good.isGood);

  if (good.delta == 0) {
    if (IntersectBadWithGood(bad, good).size() == 0) {
      return AngleVector {bad, good}; 
    }
    return AngleVector {bad};
  }

  Angle realBad = good.Ñomplement();
  Angle realGood = bad.Ñomplement();

  if (realBad.isEmpty || realGood.isFull)
    return vector<Angle>{Angle(0, PI2)};

  vector<Angle> goods1 = getGoods(realBad);
  vector<Angle> goods2{ realGood };

  //get intersect res
  AngleSet interRes(IntersectGoodVecWithGoodVec(goods1, goods2));
  //get complement for vector<Angle> interRes
  return interRes.ComplementSelf();
}


std::vector<Angle> Angle::UnionBadWithBad(const Angle &fst, const Angle &scnd)
{
  assert(!(fst.isGood) && !(scnd.isGood));

  Angle realGood1 = fst.Ñomplement();
  Angle realGood2 = scnd.Ñomplement();

  if (realGood1.isEmpty || realGood2.isFull)
    return vector<Angle>{Angle(0, PI2)};

  vector<Angle> goods1{ realGood1 };
  vector<Angle> goods2{ realGood2 };

  //get intersect res
  AngleSet interRes(IntersectGoodVecWithGoodVec(goods1, goods2));
  //get complement for vector<Angle> interRes
  return interRes.ComplementSelf();
}


std::vector<Angle> Angle::Union(const Angle &An)
{
  if (isEmpty)
    if (An.isEmpty)
      return std::vector<Angle>();
    else
      return std::vector<Angle>{An};
  else if (An.isEmpty)
    return std::vector<Angle>{*this};

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


Angle Angle::Ñomplement() const
{
  if (isFull)
    return Angle();
  if (isEmpty || Equal(delta, 0))
    return Angle(0, PI2);
  return Angle(start + delta, start);
}