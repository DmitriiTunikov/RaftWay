#pragma once
#include "vector2d.h"
#include "angle_graph_elem.h"
#include "graph.h"
#include <map>
#include "CameFromElem.h"

class WayPoint{
public:
  WayPoint(Vector2d p, double a) : pos(p), angle(a){}
  Vector2d pos;
  double angle;
};


class Dijkstra
{
public:
  static std::vector<WayPoint> GetGoalWay(AngleGraphElem& start, AngleGraphElem& goal, std::vector<CameFromElem> &came_from);
  static bool GetAllWays(AngleGraphElem& start, AngleGraphElem& goal, Graph<AngleGraphElem>& graph, 
    std::vector<CameFromElem> &came_from);
};