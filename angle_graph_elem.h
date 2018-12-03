#pragma once
#include "angle_set.h"
#include "vector2d.h"
#include <vector>

using namespace std;

class AngleGraphElem{
public:
  AngleGraphElem(const Vector2d& p_, const AngleSet& angleSet_) : p(p_), angleSet(angleSet_) {
  }
  AngleGraphElem() {}

  /* Getting id function */
  int getId(void) const
  {
    return id;
  } /* End of 'getId' function */

    /* Setting id function */
  void setId(const int ID)
  {
    id = ID;
  } /* End of 'setId' function */

    /* Getting position function */
  Vector2d getPos(void) const
  {
    return p;
  } /* End of 'getPos' function */

    /* Setting position function */
  void setPos(const Vector2d &Pos)
  {
    p = Pos;
  } /* End of 'setPos' function */

    /* Getting neighbors function */
  std::vector<int> getNeighbors(void) const
  {
    return neighbors;
  } /* End of 'getNeighbors' function */

    /* Adding neighbor function */
  void addNeighbor(const int ID)
  {
    neighbors.push_back(ID);
  } /* End of 'addNeighbor' function */

  void setCurAngle(const Angle& ang){
    curAngle = ang;
  }

  AngleSet& getAngleSet(){
    return angleSet;
  }

  const Angle& getCurAngle() const {
    return curAngle;
  }

  void setNeighbors(const vector<int> neighs){
    neighbors = neighs;
  }

  void SetAngleSet(const AngleSet& angleSet_){
    angleSet = angleSet_;
  }


  AngleGraphElem& operator= (AngleGraphElem& elem){
    p = elem.getPos();
    angleSet = elem.getAngleSet();
    id = elem.getId();
    neighbors = elem.getNeighbors();
    curAngle = elem.curAngle;
    curRealAngle = elem.curRealAngle;
    return *this;
  }

public:
  int id;
  Vector2d p;
  AngleSet angleSet;
  Angle curAngle;
  double curRealAngle;
  vector<int> neighbors;
};