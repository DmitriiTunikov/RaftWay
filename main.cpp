#include "world.h"
#include <string>

//#include "win.h"
//#include "anim.h"
#include "solve_nonlinear_equation.h"

//static World world(1, Vector2d(-0.5, -0.5), Vector2d(0, 1), 0.4, 0.1, 0.1);

static void Draw()
{
  //Anim2d::Start();  
  //
  ////Draw world
  //for (unsigned i = 0; i < world.GetObstacles().size(); i++)
  //  Anim2d::DrawSegment(world.GetObstacles()[i], Red, 1);
  //
  //for (unsigned i = 0; i < world.raft.sides.size() - 1; i++)
  //  Anim2d::DrawSegment(*(Segment *)world.raft.sides[i], Red, 1);
  //
  //
  //Anim2d::End();
}

int main(int argc, char* argv[])
{
  //win::Win::Create(&argc, argv, "Hello");
  //win::Win::DisplayFuncSet(Draw);
  //win::Win::IdleFuncSet(Draw);
  //
  //win::Win::Run();
  //
  //AngleSet res;
  //EquationSolve(Segment(Vector2d(-1, -1), Vector2d(-1, 1)), Vector2d(-0.95, 0), Segment(Vector2d(-.05, -1), Vector2d(-.05, 1)), res);
  std::vector<WayPoint> way;
  World world(1, Vector2d(0, 0), Vector2d(0, 1), 0.4, 0.1, 0.3);
  double t = clock();

  world.ComputeWay(way, 0.01);

  for (WayPoint &p : way)
  {
    cout << p.pos.GetX() << " " << p.pos.GetY() << endl;
  }
  t = (clock() - t) / CLOCKS_PER_SEC;
  cout << "Full build time: " << t << endl;

  //double minRaftSide, maxRaftSide;
  //double minTriangleSide, maxTriangleSide;
  //double minHalfCircleRadius, maxHalfCircleRadius;
  //double epsilon = 0.1;
  system("pause");



 /* double maxSquare = 0;
  for (double raftSide = minRaftSide; raftSide <= maxRaftSide; raftSide += epsilon) {
    for (double raftSide = minRaftSide; raftSide <= maxRaftSide; raftSide += epsilon) {
      for (double raftSide = minRaftSide; raftSide <= maxRaftSide; raftSide += epsilon) {
        //World world(1, Vector2d(0, 0), Vector2d(0, 1), 0.35, 0.05, 0.1);

      }
    }
  }*/

}