#pragma once

#include "angle_set.h"
#include "vector2d.h"

#include <math.h>

Angle EquationSolveMore(double SinCoef,
  double CosCoef,
  double RightPart);

static Angle EquationSolveLess(double SinCoef,
  double CosCoef,
  double RightPart);

static bool EquationSolve( const Segment &Seg,
                           const Vector2d &Center,
                           const Segment &Relative,
                           AngleSet &Res)
{
  Vector2d P1 = Seg.GetBegin();
  Vector2d P2 = Seg.GetEnd();
  Vector2d a1 = Center - P1;
  Vector2d P1P2 = P2 - P1;


  double k = P1P2.GetX(), 
         l = P1P2.GetY();

  double right1 = l * a1.GetX() - k * a1.GetY();
  double sinCoef1 = k * Relative.GetBegin().GetX() + l * Relative.GetBegin().GetY();
  double cosCoef1 = k * Relative.GetBegin().GetY() - l * Relative.GetBegin().GetX();

  Angle MoreZero1 = EquationSolveMore(sinCoef1, cosCoef1, right1);
  Angle LessZero1 = MoreZero1.Ñomplement();

  double right2 = l * a1.GetX() - k * a1.GetY();
  double sinCoef2 = k * Relative.GetEnd().GetX() + l * Relative.GetEnd().GetY();
  double cosCoef2 = k * Relative.GetEnd().GetY() - l * Relative.GetEnd().GetX();
  Angle MoreZero2 = EquationSolveMore(sinCoef2, cosCoef2, right2);
  Angle LessZero2 = MoreZero2.Ñomplement();

  Vector2d P2P1 = P1 - P2;
  k = P2P1.GetX(),
  l = P2P1.GetY();
  double right3 = l * a1.GetX() - k * a1.GetY();
  double sinCoef3 = k * Relative.GetBegin().GetX() + l * Relative.GetBegin().GetY();
  double cosCoef3 = k * Relative.GetBegin().GetY() - l * Relative.GetBegin().GetX();

  Angle MoreZero3 = EquationSolveMore(sinCoef1, cosCoef1, right1);
  Angle LessZero3 = MoreZero1.Ñomplement();

  double right4 = l * a1.GetX() - k * a1.GetY();
  double sinCoef4 = k * Relative.GetEnd().GetX() + l * Relative.GetEnd().GetY();
  double cosCoef4 = k * Relative.GetEnd().GetY() - l * Relative.GetEnd().GetX();
  Angle MoreZero4 = EquationSolveMore(sinCoef2, cosCoef2, right2);
  Angle LessZero4 = MoreZero2.Ñomplement();

  // Combinate angles to get multiplication less zero
  AngleVector res1 = MoreZero1.Intersect(MoreZero2);
  AngleVector res2 = LessZero1.Intersect(LessZero2);

  AngleVector res3 = MoreZero3.Intersect(MoreZero4);
  AngleVector res4 = LessZero4.Intersect(LessZero3);
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
  double alpha = acos(a);
  double phi = acos(c);
  return Angle(-phi - alpha, phi - alpha);
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
