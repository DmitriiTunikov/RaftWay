#pragma once

#include "angle_set.h"
#include <fstream>
#include "vector2d.h"
#include <string>
#include "amp_math.h"
#include <stdio.h> 
#include <direct.h>
#include <algorithm>


#include <math.h>
#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif

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
  
  Vector2d aa = Relative.GetEnd() - Relative.GetBegin();
  double a = aa.GetX();
  double b = aa.GetY();
  Vector2d kk = Seg.GetBegin() - Center;
  k = kk.GetX();
  l = kk.GetY();


  double cos2Coef = -a * Relative.GetBegin().GetY() + Relative.GetBegin().GetX() * b;//-a.GetX() * Relative.GetBegin().GetY() + Relative.GetBegin().GetX() * a.GetY();
  double sin2Coef = b * Relative.GetBegin().GetX() - a * Relative.GetBegin().GetY();//a.GetY() * Relative.GetBegin().GetX() - a.GetX() * Relative.GetBegin().GetY();
  double sinCoef = -b * l - a * k;//-a.GetY() * kk.GetY() - a.GetX() * kk.GetX();
  double cosCoef = a * l - b * k;//a.GetX() * kk.GetY() - a.GetY() * kk.GetX();

  //first point
  AngleVector moreRes1, lessRes1;
  QuadricEquationSolveMore(cos2Coef, sin2Coef, cosCoef, sinCoef, moreRes1, lessRes1);
  //second point
  kk = Seg.GetEnd() - Center;
  k = kk.GetX();
  l = kk.GetY();
  cosCoef = -b * l - a * k;//a.GetX() * kk.GetY() - a.GetY() * kk.GetX();
  sinCoef = a * l - b * k;//-a.GetY() * kk.GetY() - a.GetX() * kk.GetX();
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
  Res.IntersectSetWithVector(moreRes1Set.GetAngleSetVec());

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
  // Solve division problems
  if (abs(a) > 1.0)
    a = Signum(a);
  // Solve equation
  // cos(alpha + phi) = c
  // alpha = acos(a)
  double alpha = acos(a);
  double phi = acos(c);
  double start = -phi + alpha;
  double end = phi + alpha;

  if (getSinCosVal((start + end) / 2, SinCoef, CosCoef, RightPart) > 0)
    return Angle(start, end);
  else
    return Angle(-phi - alpha, phi - alpha);

}

double CalculateQuadric(double x, double a, double b,  double d, double e)
{
  // a * cos^2(x) + b * sin^2(x) + c * cos(x) * sin(x) + d cos(x) + e sin(x)
  double si = sin(x), co = cos(x);
  return a * co * co + b * si * si + d * co + e * si;
}

string imp1 = "from math import pi";
string sl = "\n";

void QuadricEquationSolveMore(double a, double b, double d, double e, AngleVector& moreRes, AngleVector& lessRes)
{
  char current_work_dir[FILENAME_MAX];
  _getcwd(current_work_dir, sizeof(current_work_dir));
  string curDir(current_work_dir);
  string resFile = "res.txt";

  //solve equation and write res to file resFile
  std::string s("C:\\Python27\\python \"" + curDir + "\\equation_solve.py\" " + std::to_string(a) + " " + std::to_string(b) 
   + " " + std::to_string(d) + " " + std::to_string(e) + " " + resFile);
  system(s.c_str());

  //read res
  std::ifstream resF(resFile);
  int angleCount = 0;
  resF >> angleCount;
  vector<double> anglesDouble;
  for (int k = 0; k < angleCount; k++){
    double d = 0;
    resF >> d;
    anglesDouble.push_back(d);
  }
  resF.close();

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
  for (int i = 0; i < anglesDouble.size() - 1; i++){
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