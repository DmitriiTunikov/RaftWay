/*
 * Motion planning problem project.
 *
 * FILE: support.h
 * AUTHORS:
 *   Denisov Pavel
 *   Tunikov Dmitrii
 * LAST UPDATE: 19.05.2018
 * NOTE: support class declaration file
 */

#pragma once
#include <cmath>
#include <iostream>
#include <vector>
#include "Vector2d.h"

/* Support namespace */
namespace sup
{
  // Contants
  const float eps = 1e-4f;
  const float pi = 3.14159265359f;
  using Vecd = Vector2d;

  /* Line structure */
  struct Line
  {
    float _k, _b;

    /* Constructor */
    Line(float k = 0, float b = 0) : _k(k), _b(b)
    {
    } /* End of constructor */
  }; /* End of 'Line' structure */


  /* Getting f(x) by line structure */
  inline float getFX(const Line &Line, const float X)
  {
    return Line._k * X + Line._b;
  } /* End of 'getFX' function */

  /* Finding element in array by id */
  template <class T>
  inline T & findElemInArrayById(std::vector<T> &Arr, const int Id)
  {
    for (size_t i = 0; i < Arr.size(); i++)
      if (Arr[i].getId() == Id)
        return Arr[i];
  } /* End of 'findElemInArrayById' function */

  /* Finding element in array by id */
  template <class T>
  inline int findElemIndexInArrayById(std::vector<T> &Arr, const int Id)
  {
    for (size_t i = 0; i < Arr.size(); i++)
      if (Arr[i].getId() == Id)
        return i;
  } /* End of 'findElemIndexInArrayById' function */
} /* end of 'sup' namespace */

/* END OF 'support.h' FILE */
