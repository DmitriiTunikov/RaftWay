#pragma once

#include "angle_set.h"
#include "vector2d.h"

#include <math.h>

static bool EquationSolve( const Segment &Seg,
                           const Vector2d &Center,
                           const Segment &Relative,
                           AngleSet &Res)
{
  Vector2d P1 = Seg.GetBegin();
  Vector2d P2 = Seg.GetEnd();
  Vector2d delta1 = Center - P1;
  Vector2d delta2 = Center - P2;
  Vector2d P1P2 = P2 - P1;
  Vector2d ab = Relative.GetBegin() - Center;
  Vector2d cd = Relative.GetEnd() - Center;

  double k = P1P2.GetX(), 
         l = P1P2.GetY();

  double right1 = l * delta1.GetX() - k * delta1.GetY();
  double sinCoef1 = k * cd.GetX() - l * cd.GetY();
  double cosCoef1 = k * cd.GetY() + l * cd.GetX();
  Angle MoreZero1 = EquationSolveMore(sinCoef1, cosCoef1, right1);
  Angle LessZero1 = MoreZero1.Ñomplement();

  double right2 = l * delta2.GetX() - k * delta2.GetY();
  double sinCoef2 = k * ab.GetX() - l * ab.GetY();
  double cosCoef2 = k * ab.GetY() + l * ab.GetX();
  Angle MoreZero2 = EquationSolveMore(sinCoef2, cosCoef2, right2);
  Angle LessZero2 = MoreZero2.Ñomplement();
  // Combinate angles to get multiplication less zero
  AngleVector res1 = MoreZero1.Intersect(LessZero2);
  AngleVector res2 = LessZero1.Intersect(MoreZero2);
  Res = AngleSet(res1);
  Res.UnionSetWithVector(res2);
  return !Res.Empty();
}

static double Signum(double A)
{
  return A > 0 ? 1 : A < 0 ? -1 : 0;
}

Angle EquationSolveMore(double SinCoef,
                        double CosCoef,
                        double RightPart)
{
  double a = SinCoef,
         b = CosCoef,
         c = RightPart;

  double mult = 1.0 / sqrt(a * a + b * b);

  c *= mult;
  if (abs(c) > 1.0)
    if (c < -1.0)
    {
      return Angle(0, 2 * M_PI);
    }
    else
      return Angle();
  a *= mult;
  // Solve division problems
  if (abs(a) > 1.0)
    a = Signum(a);
  // Solve equation
  // cos(alpha + phi) = c
  // alpha = acos(a)
  double phi = acos(c) - acos(a);
  return Angle(-phi, phi);
}

static Angle EquationSolveLess(double SinCoef,
  double CosCoef,
  double RightPart)
{
  double a = SinCoef,
    b = CosCoef,
    c = RightPart;

  double mult = 1.0 / sqrt(a * a + b * b);

  c *= mult;
  if (abs(c) > 1.0)
    if (c > 1.0)
    {
      return Angle(0, 2 * M_PI);
    }
    else
      return Angle();
  a *= mult;
  // Solve division problems
  if (abs(a) > 1.0)
    a = Signum(a);
  // Solve equation
  // cos(alpha + phi) = c
  // alpha = acos(a)
  double phi = acos(c) - acos(a);
  return Angle(phi, -phi);
}
