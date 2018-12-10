#include "world.h"
#include <string>

int main(void)
{
  Angle sos = LinearEquationSolveMore(1, -1, 0);

  AngleSet res;
  EquationSolve(Segment(Vector2d(-1, -1), Vector2d(-1, 0.5)), Vector2d(-.95, 0.5), Segment(Vector2d(0, -1), Vector2d(0, 1)), res);
  std::vector<WayPoint> way;
  World world(1, Vector2d(0, 0), Vector2d(0, 1), 0.4, 0.1, 0.1);
  double minRaftSide, maxRaftSide;
  double minTriangleSide, maxTriangleSide;
  double minHalfCircleRadius, maxHalfCircleRadius;
  double epsilon = 0.1;

  world.ComputeWay(way, epsilon);

 /* double maxSquare = 0;
  for (double raftSide = minRaftSide; raftSide <= maxRaftSide; raftSide += epsilon) {
    for (double raftSide = minRaftSide; raftSide <= maxRaftSide; raftSide += epsilon) {
      for (double raftSide = minRaftSide; raftSide <= maxRaftSide; raftSide += epsilon) {
        //World world(1, Vector2d(0, 0), Vector2d(0, 1), 0.35, 0.05, 0.1);

      }
    }
  }*/

}