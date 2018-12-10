#include "world.h"
#include <string>
#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif
int main(void)
{
  /*Graph<AngleGraphElem> gr;
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
  */

  /*std::string pythonCode = "import time\n time.sleep(100)   ";
  system((std::string("C:\\Python27\\python -c \"") + pythonCode + "\"").c_str());
  system(pythonCode.c_str());*/
  Py_Initialize();

  Angle sos = LinearEquationSolveMore(1, -1, 0);

  AngleSet res;
  EquationSolve(Segment(Vector2d(-1, -1), Vector2d(-1, 1)), Vector2d(-.98, 0), Segment(Vector2d(0, -2), Vector2d(0, 2)), res);
  std::vector<WayPoint> way;
  World world(1, Vector2d(0, 0), Vector2d(0, 1), 0.35, 0.05, 0.1);
  double minRaftSide, maxRaftSide;
  double minTriangleSide, maxTriangleSide;
  double minHalfCircleRadius, maxHalfCircleRadius;
  double epsilon = 0.1;

  world.ComputeWay(way, epsilon);
  Py_Finalize();

 /* double maxSquare = 0;
  for (double raftSide = minRaftSide; raftSide <= maxRaftSide; raftSide += epsilon) {
    for (double raftSide = minRaftSide; raftSide <= maxRaftSide; raftSide += epsilon) {
      for (double raftSide = minRaftSide; raftSide <= maxRaftSide; raftSide += epsilon) {
        //World world(1, Vector2d(0, 0), Vector2d(0, 1), 0.35, 0.05, 0.1);

      }
    }
  }*/

}