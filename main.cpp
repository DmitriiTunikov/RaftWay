
#include "graph.h"
#include "angle_graph_elem.h"
#include "dijkstra.h"
#define _USE_MATH_DEFINES // for C  
#include <math.h>  

int main(void)
{
  Graph<AngleGraphElem> gr;
  AngleSet as;
  as.GetAngleSetVec().push_back(Angle(0, M_PI / 3));
  as.GetAngleSetVec().push_back(Angle(M_PI, M_PI  + M_PI / 3));
  gr.addElem(AngleGraphElem(Vector2d(0, 0), as));

  AngleSet as1;
  as1.GetAngleSetVec().push_back(Angle(0, M_PI / 4));
  as1.GetAngleSetVec().push_back(Angle(M_PI, M_PI + M_PI / 6));
  gr.addElem(AngleGraphElem(Vector2d(1, 1), as1));
  
  gr.addLink(0, 1);
  std::vector<AngleGraphElem> came_from;
  bool haveWay = Dijkstra::GetAllWays(gr.getElements()[0], gr.getElements()[gr.getElements().size() - 1], gr, came_from);
  std::vector<WayPoint> way = Dijkstra::GetGoalWay(gr.getElements()[0], gr.getElements()[gr.getElements().size() - 1], came_from);
}