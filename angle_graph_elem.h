#pragma once
#include "angle_set.h"
#include "vector2d.h"
#include <vector>

using namespace std;

class AngleGraphElem{
  AngleGraphElem(const Vector2d& p_, const AngleSet& angleSet_) : p(p_), angleSet(angleSet_) {
  }

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

private:
  int id;
  Vector2d p;
  AngleSet angleSet;
  AngleSet curAngleSet;
  vector<int> neighbors;
};