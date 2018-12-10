#pragma once
#include <utility>
#include <vector>
#include <algorithm>
#include <math.h>
#include "angle.h"
#include <assert.h>

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
    deleteEmptyAngles();
  }

  void deleteEmptyAngles(){
    for (unsigned i = 0; i < _set.size();){
      if (_set[i].isEmpty){
        _set.erase(_set.begin() + i);
      }
      else
        i++;
    }
  }

  void UnionSelf(const Angle& An){
    if (_set.size() == 0)
    {
      _set.push_back(An);
      return;
    }

    for (unsigned i = 0; i < _set.size(); i++)
    {
      AngleVector un = _set[i].Union(An);
      if (un.size() == 2)
        continue;
      _set.erase(_set.begin() + i);
      UnionSelf(un[0]);
      return;
    }

    _set.push_back(An);
  }

  bool Intersected(const Angle &Ang, AngleSet &Res, std::vector <double> &realAngles)
  {
    Res._set.clear();

    for (unsigned int i = 0; i < _set.size();)
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

  AngleVector IntersectSelf(const Angle &An)
  {
    AngleVector res;
    AngleVector res1;

    for (Angle& AnglSelf : _set)
    {
      res1 = AnglSelf.Intersect(An);
      res.insert(res.end(), res1.begin(), res1.end());
    }
    return res;
  }

  void IntersectSetWithVector(const AngleVector &AnVec)
  {
    AngleVector res;
    for (const Angle &AngleO : AnVec) 
    {
      AngleVector resT = IntersectSelf(AngleO);
      res.insert(res.end(), resT.begin(), resT.end());
    }
    _set = res;
    deleteEmptyAngles();
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
