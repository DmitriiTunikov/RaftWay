#pragma once
#include <vector>
#include <assert.h>


static double binSerarch(std::function<double(double)> Func, double A, double B, double Left, double Right, double Precision)
{
  if (Left > 0)
  {
    std::swap(A, B);
    std::swap(Left, Right);
  }

  do
  {
    double c = (B + A) / 2;
    double val = Func(c);

    if (val == 0)
      return c;
    else if (val < 0)
      A = c;
    else
      B = c;

  } while (abs(B - A) > Precision);

  return (A + B) / 2;
}

static std::vector<double> FindRoots(std::function<double(double)> Func, double A, double B, double Precision, unsigned MaxRoots)
{
  std::vector<double> res;

  const static double step = 0.005;
  double end = B - step;
  
  double left = Func(A);
  for (double i = A; i < end && res.size() < MaxRoots; i += step)
  {
    double right = Func(i);
    if (left * right < 0)
      res.push_back(binSerarch(Func, i - step, i, left, right, Precision));
    left = right;
  }
  return res;
}
