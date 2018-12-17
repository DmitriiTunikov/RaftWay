#pragma once

#include "angle_set.h"
#include <fstream>
#include "vector2d.h"
#include <string>
#include "amp_math.h"
#include <stdio.h> 
#include <direct.h>
#include <algorithm>
#include "solve_nonlinear_equation.h"

void QuadricEquationSolveMore(double a, double b, double d, double e, AngleVector& moreRes, AngleVector& lessRes);
Angle LinearEquationSolveMore(double SinCoef,
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

  Angle MoreZero1 = LinearEquationSolveMore(sinCoef1, cosCoef1, right1);
  Angle LessZero1 = MoreZero1.Ñomplement();

  double right2 = l * a1.GetX() - k * a1.GetY();
  double sinCoef2 = k * Relative.GetEnd().GetX() + l * Relative.GetEnd().GetY();
  double cosCoef2 = k * Relative.GetEnd().GetY() - l * Relative.GetEnd().GetX();
  Angle MoreZero2 = LinearEquationSolveMore(sinCoef2, cosCoef2, right2);
  Angle LessZero2 = MoreZero2.Ñomplement();

  // Combinate angles to get multiplication less zero
  AngleVector res1 = MoreZero1.Intersect(MoreZero2);
  AngleVector res2 = LessZero1.Intersect(LessZero2);
  
  Res = AngleSet(res1);
  Res.UnionSetWithVector(res2);

  //first point
  Vector2d a = Relative.GetEnd() - Relative.GetBegin();
  double ax = a.GetX(), ay = a.GetY();
  Vector2d kk = Seg.GetBegin() - Center;
  double kx = kk.GetX();
  double ky = kk.GetY();
  double cos2Coef = -ax * Relative.GetBegin().GetY() + Relative.GetBegin().GetX() * ay;
  double sin2Coef = ay * Relative.GetBegin().GetX() - ax * Relative.GetBegin().GetY();
  double sinCoef = -ay * ky - ax * kx;
  double cosCoef = ax * ky - ay * kx;
  AngleVector moreRes1, lessRes1;
  QuadricEquationSolveMore(cos2Coef, sin2Coef, cosCoef, sinCoef, moreRes1, lessRes1);

  //second point 
  kk = Seg.GetEnd() - Center;
  kx = kk.GetX();
  ky = kk.GetY();
  sinCoef = -ay * ky - ax * kx;
  cosCoef = ax * ky - ay * kx;
  AngleVector moreRes2, lessRes2;
  QuadricEquationSolveMore(cos2Coef, sin2Coef, cosCoef, sinCoef, moreRes2, lessRes2);

  //make multiplication more zero!
  AngleSet moreRes1Set(moreRes1);
  AngleSet lessRes1Set(lessRes1);
  moreRes1Set.IntersectSetWithVector(moreRes2);
  lessRes1Set.IntersectSetWithVector(lessRes2);

  //union results
  moreRes1Set.UnionSetWithVector(lessRes1Set.GetAngleSetVec());

  //intersect Res with Res2
  Res.UnionSetWithVector(moreRes1Set.GetAngleSetVec());
  //delete empty
  return !Res.Empty();
}

static double Signum(double A)
{
  return A > 0 ? 1 : A < 0 ? -1 : 0;
}

double getSinCosVal(double x, double SinCoef, double CosCoef, double RightPart){
  return sin(x) * SinCoef + cos(x) * CosCoef - RightPart;
}

Angle LinearEquationSolveMore(double SinCoef,
                              double CosCoef,
                              double RightPart)
{
  double b = SinCoef,
    a = CosCoef,
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
  b *= mult;
  // Solve division problems
  if (abs(a) > 1.0)
    a = Signum(a);
  // Solve equation
  // cos(alpha + phi) = c
  // alpha = acos(a)
  double alpha = acos(a);
  double phi = acos(c);
  if (Signum(sin(alpha)) != Signum(b))
    alpha = -alpha;

  return Angle(-phi - alpha, phi - alpha);
}

double CalculateQuadric(double x, double a, double b,  double d, double e)
{
  // a * cos^2(x) + b * sin^2(x) + c * cos(x) * sin(x) + d cos(x) + e sin(x)
  double si = sin(x), co = cos(x);
  return a * co * co + b * si * si + d * co + e * si;
}

void QuadricEquationSolveMore(double a, double b, double d, double e, AngleVector& moreRes, AngleVector& lessRes)
{
  std::function<double(double)> func = [a, b, d, e](double x){double co = cos(x), si = sin(x); return a * co * co + b * si * si + d * co + e * si;};

  vector<double> anglesDouble = FindRoots(func, 0, PI2, 0.0001, 2);
  
  if (anglesDouble.size() == 0)
  {

    if (CalculateQuadric(M_PI, a, b, d, e) > 0)
      moreRes.push_back(Angle(0, PI2));
    else
      lessRes.push_back(Angle(0, PI2));
    return;
  }

  std::sort(anglesDouble.begin(), anglesDouble.end());
  
  //center
  for (unsigned i = 0; i < anglesDouble.size() - 1; i++){
    double center = (anglesDouble[i] + anglesDouble[i + 1]) / 2;

    if (CalculateQuadric(center, a, b, d, e) > 0)
      moreRes.push_back(Angle(anglesDouble[i], anglesDouble[i + 1]));
    else
      lessRes.push_back(Angle(anglesDouble[i], anglesDouble[i + 1]));
  }
  if (CalculateQuadric(anglesDouble[anglesDouble.size() - 1] + anglesDouble[0]/2 , a, b, d, e) > 0)
    moreRes.push_back(Angle(anglesDouble[anglesDouble.size() - 1], anglesDouble[0]));
  else
    lessRes.push_back(Angle(anglesDouble[anglesDouble.size() - 1], anglesDouble[0]));
}