#pragma once
#include <utility>
#include <vector>
#include <algorithm>
#include <math.h>
#include "angle.h"
#include <assert.h>
using namespace std;

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

  AngleVector ComplementSelf() const {
    AngleSet res;
    //initially complement is 0 -> 2pi angle
    res._set.push_back(Angle(0, PI2));

    //get all complements and intersect with current res
    for (const Angle& an : _set){
      Angle complementAn = an.Ñomplement();
      res = res.IntersectSelf(complementAn);
    }
    return res._set;
  }

  bool Intersected(const Angle &Ang, AngleSet &Res, std::vector <Angle> &realAngles)
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
      //realAngles.push_back(inter[0].start + inter[0].delta / 2);
      realAngles.push_back(inter[0]);
      _set.erase(_set.begin() + i);
    }
    return realAngles.size() > 0;
  }

  static std::vector<Angle> UnionBadWithGood(const Angle &bad, const Angle &good)
  {
    assert(!(bad.isGood) && good.isGood);

    Angle realBad = good.Ñomplement();
    Angle realGood = bad.Ñomplement();

    vector<Angle> goods1 = Angle::getGoods(realBad);
    vector<Angle> goods2{ realGood };

    //get intersect res
    AngleSet interRes(Angle::IntersectGoodVecWithGoodVec(goods1, goods2));
    //get complement for vector<Angle> interRes
    return interRes.ComplementSelf();
  }

  static std::vector<Angle> UnionBadWithBad(const Angle &fst, const Angle &scnd)
  {
    assert(!(fst.isGood) && !(scnd.isGood));

    Angle realGood1 = fst.Ñomplement();
    Angle realGood2 = scnd.Ñomplement();

    vector<Angle> goods1{ realGood1 };
    vector<Angle> goods2{ realGood2 };

    //get intersect res
    AngleSet interRes(Angle::IntersectGoodVecWithGoodVec(goods1, goods2));
    //get complement for vector<Angle> interRes
    return interRes.ComplementSelf();
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
