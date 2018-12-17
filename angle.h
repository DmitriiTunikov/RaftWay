#pragma once

#include <utility>
#include <vector>
#include <algorithm>
#include <assert.h>
#define _USE_MATH_DEFINES // for C  
#include <math.h>  

const static double PI2 = M_PI * 2;
const double EPSI = 0.005;

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
    if (Equal(delta, 0)) {
      delta = 0;
      while (start < 0)
        start += PI2;
      while (start > PI2)
        start -= PI2;
      return;
    }
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

  static std::vector<Angle> getGoods(const Angle& an);
  
  static Angle IntersectGoodWithGood(const Angle &fst, const Angle &scnd);

  static std::vector<Angle> IntersectBadWithGood(const Angle &bad, const Angle &good);

  static std::vector<Angle> IntersectGoodVecWithGoodVec(const std::vector<Angle>& goods1, const std::vector<Angle>& goods2);

  static std::vector<Angle> IntersectBadWithBad(const Angle &fst, const Angle &scnd);

  std::vector<Angle> Intersect(const Angle &An);

  static std::vector<Angle> UnionGoodWithGood(const Angle &fst, const Angle &scnd);

  static std::vector<Angle> UnionBadWithGood(const Angle &bad, const Angle &good);

  static std::vector<Angle> UnionBadWithBad(const Angle &fst, const Angle &scnd);


  std::vector<Angle> Union(const Angle &An);

  Angle Ñomplement() const;
};
