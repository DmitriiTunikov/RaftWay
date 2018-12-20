#include "world.h"
#include <string>

#include "solve_nonlinear_equation.h"

static const double sqr34 = sqrt(3) / 4;

static const string fileName = "calculate.txt";
static const string fileNameCalculated = "last_calculated.txt";
static const string wayFileName = "way.txt";

inline static double Square(double raftSide, double triangleSide, double CircleRadius)
{
  return raftSide * raftSide + sqr34 * triangleSide * triangleSide - M_PI * CircleRadius * CircleRadius / 2;
}

int main(int argc, char* argv[])
{
  double epsi = 0.01;
  std::vector<WayPoint> way;
  double maxSquare = 0;
  //double raftSideGlobal, triangleSideGlobal, circleGlobal;

  //ifstream fin(fileName);
  //
  //fin >> raftSideGlobal >> triangleSideGlobal >> circleGlobal >> maxSquare;
  //
  //fin.close();

  World world(1, epsi);

  for (double raftSide = 1.0 - epsi; raftSide > 0; raftSide -= epsi)
    for (double triangleSide = raftSide; triangleSide >= 0; triangleSide -= epsi)
      for (double circle = 0; circle <= 0.5 * raftSide; circle += epsi)
      {
        double curSquare = Square(raftSide, triangleSide, circle);
        if (curSquare <= maxSquare)
          continue;
        world.UpdateWorld(raftSide, triangleSide, circle);
        cout << raftSide << " " << triangleSide << " " << circle << endl;
        if (world.ComputeWay(way))
        {
          cout << "Way found" << endl;

          maxSquare = curSquare;

          // Write out data
          ofstream fout(fileName);
          fout << raftSide << " " << triangleSide << " " << circle << " ";
          fout << maxSquare;
          fout.close();
          // write way data
          ofstream foutWay(wayFileName);
          foutWay << way.size() << endl;
          for (WayPoint &p : way)
          {
            foutWay << p.pos.GetX() << " " << p.pos.GetY() << " ";
            foutWay << p.angle << endl;
          }
          foutWay.close();
        }
        // Write out data
        //ofstream fout(fileNameCalculated);
        //fout << raftSide << " " << triangleSide << " " << circle << " ";
        //fout.close();
      }
  system("pause");
}