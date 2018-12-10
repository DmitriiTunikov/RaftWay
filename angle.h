#pragma once

#include <utility>
#include <vector>
#include <algorithm>

#include <assert.h>
#define _USE_MATH_DEFINES // for C  
#include <math.h>  

const static double PI2 = M_PI * 2;
const double EPSI = 0.0001;

static inline bool Less(double a, double b)
{
  return a - EPSI <= b;
}

static inline bool More(double a, double b)
{
  return a + EPSI >= b;
}

static inline bool Equal(double a, double b){
  return More(a, b) && Less(a, b);
}

class Angle
{
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
    isEmpty(false), isFull(false),
    isGood(true)
  {
    while (More(delta, PI2))
      delta -= PI2;
    if (Equal(delta, 0)){
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

    if (start + delta > PI2)
      isGood = false;
  }

  static std::vector<Angle> IntersectGoodWithGood(const Angle &fst, const Angle &scnd)
  {
    assert(fst.isGood && scnd.isGood);
    std::vector<Angle> res;
    double a = fst.start;
    double b = fst.start + fst.delta;
    double c = scnd.start;
    double d = scnd.start + scnd.delta;

    if (Less(a, c) && Less(d, b)) {
      res.push_back(Angle(c, d));
    }
    else if (Less(c, a) && Less(b, d)) {
      res.push_back(Angle(a, b));
    }
    else if (More(c, a) && More(d, b) && More(b, c)) {
      res.push_back(Angle(c, b));
    }
    else if (Less(c, a) && Less(d, b) && More(d, a)) {
      res.push_back(Angle(a, d));
    }

    return res;
  }

  static std::vector<Angle> IntersectBadWithGood(const Angle &bad, const Angle &good)
  {
    assert(!(bad.isGood) && good.isGood);

    double a = good.start;
    double b = good.start + good.delta;
    double c = bad.start;
    double d = bad.start + bad.delta - PI2;
    std::vector<Angle> res;

    if (good.isFull) 
    {
      res.push_back(bad);
    }

    else if (Less(c, b) && More(c, a) && Less(d,a)) {
      res.push_back(Angle(c, b));
    }

    else if (Less(b, c) && Less(a , d) && Less(d, b)) {
      res.push_back(Angle(a, d + PI2));
    }

    else if ((More(d, b) && More(c , b)) || (Less(d, a) && Less(c, a))) {
      res.push_back(Angle(a, b));
    }
    else if (Less(d, a) && Less(b, c)){
      return res;
    }

    else if (Less(c, b) && More(d, a)) {
      res.push_back(Angle(a, d + PI2));
      res.push_back(Angle(c, b));
    }
    else {
      assert(false && "Bad with good intersection failed");
    }
    return res;
  }

  static std::vector<Angle> IntersectBadWithBad(const Angle &fst, const Angle &scnd)
  {
    assert(!(fst.isGood) && !(scnd.isGood));

    double a = fst.start;
    double b = fst.start + fst.delta - PI2;
    double c = scnd.start;
    double d = scnd.start + scnd.delta - PI2;
    std::vector<Angle> res;

    if (Less(d, b) && More(c, a)) {
       res.push_back(scnd);
     }

    else if (More(d, b) && More(c, a) && Less(d,a)) {
      res.push_back(Angle(c, b + PI2));
    }

    else if (Less(d, b) && Less(c, a) && More(c, b)) {
      res.push_back(Angle(a, d + PI2));
    }

    else if (Less(b, d) && Less(c, a)) {
      res.push_back(Angle(a, b + PI2));
    }
    else if (More(d, a) && More(c, a)) {
      res.push_back(Angle(c, b + PI2));
      res.push_back(Angle(a, d + PI2));
    }
    else if (Less(d, b) && Less(c, b)) {
      res.push_back(Angle(c, b + PI2));
      res.push_back(Angle(a, d + PI2));
    }
    else {
      assert(false && "Bad with Bad Failed");
    }
    return res;
  }

  std::vector<Angle> Intersect(const Angle &An)
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

  static std::vector<Angle> UnionGoodWithGood(const Angle &fst, const Angle &scnd)
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

  static std::vector<Angle> UnionBadWithGood(const Angle &bad, const Angle &good)
  {
    assert(!(bad.isGood) && good.isGood);

    double a = good.start;
    double b = good.start + good.delta;
    double c = bad.start;
    double d = bad.start + bad.delta - PI2;
    std::vector<Angle> res;

    if (good.isFull)
    {
      res.push_back(good);
    }
    else if (Less(c, b) && More(d, a)) {
      res.push_back(Angle(0, PI2));
    }

    else if (Less(c, b) && More(c, a) && Less(d, a)) {
      res.push_back(Angle(a, d + PI2));
    }

    else if (Less(b, c) && Less(a, d) && Less(d, b)) {
      res.push_back(Angle(c, b));
    }

    else if ((More(d, b) && More(c, b)) || (Less(d, a) && Less(c, a))) {
      res.push_back(Angle(c, d + PI2));
    }

    else if (More(a, d) && Less(b, c)) {
      res.push_back(Angle(a, b));
      res.push_back(Angle(c, d + PI2));
    }


    else {
      assert(false && "Bad with good intersection failed");
    }
    return res;
  }

  static std::vector<Angle> UnionBadWithBad(const Angle &fst, const Angle &scnd)
  {
    assert(!(fst.isGood) && !(scnd.isGood));

    double a = fst.start;
    double b = fst.start + fst.delta - PI2;
    double c = scnd.start;
    double d = scnd.start + scnd.delta - PI2;
    std::vector<Angle> res;

    if (Less(d, a) && More(c, a)) {
      res.push_back(Angle(0, PI2));
    }

    else if (Less(d, b) && Less(c, b)) {
      res.push_back(Angle(0, PI2));
    }

    else if (Less(d, b) && More(c, a)) {
      res.push_back(Angle(a, b + PI2));
    }

    else if (More(d, b) && More(c, a) && Less(d, a)) {
      res.push_back(Angle(a, d + PI2));
    }

    else if (Less(d, b) && Less(c, a) && More(c, b)) {
      res.push_back(Angle(c, b + PI2));
    }

    else if (Less(b, d) && Less(c, a)) {
      res.push_back(Angle(c, d + PI2));
    }

    else {
      assert(false && "Bad with Bad Failed");
    }
    return res;
  }


  std::vector<Angle> Union(const Angle &An)
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
