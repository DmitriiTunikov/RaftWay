#pragma once

#include <vector>

#include "segment.h"
#include "raft.h"
#include "dijkstra.h"
#include "angle_graph_elem.h"
#include "graph.h"
#include "angle_equation_solve.h"

using AngleInterval = std::pair<double, double>;

class World {
public:
	World(double distanceBetweenObstacles_, Vector2d raftCenter, Vector2d front, double raftSide, double triangleSide, double halfCircleRadius)
		: raft(raftCenter, front, raftSide, triangleSide, halfCircleRadius),
      distanceBetweenObstacles(distanceBetweenObstacles_)
	{
		Vector2d leftUp(-distanceBetweenObstacles, distanceBetweenObstacles);
		Vector2d center(0, 0);

		double triangleHeight = triangleSide * sqrt(3) / 2;
		
		//left
		obstacles.push_back(Segment(-distanceBetweenObstacles,
			-raftSide - 2 * triangleHeight, leftUp.GetX(), leftUp.GetY()));
				
		//up
		obstacles.push_back(Segment(leftUp.GetX(), leftUp.GetY(),
			raftSide + 2 * triangleHeight, leftUp.GetY()));

		//inetrnal up
		obstacles.push_back(Segment(center, center + unitX * (raftSide + 2 * triangleHeight)));

		//internal left
		obstacles.push_back(Segment(center, center - unitY * (-raftSide - 2 * triangleHeight)));
	}

	const std::vector<Segment>& GetObstacles(void) {
		return obstacles;
	}

	
	bool ComputeWay(std::vector<WayPoint>& wayRes, double epsilon) {
		// raft can't be inside canal
		if (raft.raftSide + raft.triangleHeight > distanceBetweenObstacles)
			return false;

		//InitGrid();

		return true;
	}

private:
	void InitGrid(double epsilon) {
		double minX = -distanceBetweenObstacles + raft.raftSide / 2;
		double maxX = raft.raftSide / 2 + raft.triangleHeight;
		double minY = -raft.raftSide / 2 - raft.triangleHeight;
		double maxY = distanceBetweenObstacles - raft.raftSide / 2;

		double distX = maxX - minX;
		double distY = maxY - minY;

		int gridXSize = distX / epsilon;
		int gridYSize = distY / epsilon;

		//init pos in grid
    int move = gridYSize;

    bool breakFlag = false;
		for (int j = 0; j < gridXSize; j++) {
      move = gridYSize;
			for (int i = 0; i < gridYSize; i++) {				
				// point coordinate
				Vector2d pos = Vector2d(minX + distX * i, (minY + distY * j));
        if (pos.GetY() < 0 && pos.GetX() > 0) {
          move--;
          breakFlag = true;
          break;
        }
        //get angle set for current point
				AngleSet angSet = CalculateAngles(pos);
        //add elem to graph
        graph.addElem(AngleGraphElem(pos, angSet));
        if (i != 0 && !breakFlag)
          graph.addLink(graph.getElements().size() - 1, graph.getElements().size() - 2);
        if (j != 0) {
          graph.addLink(graph.getElements().size() - 1, graph.getElements().size() - move - 1);
          if (i != 0 && !breakFlag)
            graph.addLink(graph.getElements().size() - 1, graph.getElements().size() - move - 2);
          if (i != gridYSize - 1)
            graph.addLink(graph.getElements().size() - 1, graph.getElements().size() - move);
        }
        breakFlag = false;
			}
		}
	}



	AngleSet CalculateAngles(Vector2d& pos) 
  {
    AngleSet res;

    for (const Segment &Relative : raft.relativeSides)
    {
      for (const Segment &Obstacle : obstacles)
      {
        AngleSet curRes;
        if (!EquationSolve(Obstacle, pos, Relative, curRes))
        {
          return AngleSet();
        }
        if (res.Empty())
          res = AngleSet(curRes);
        else
        {
          res.IntersectSetWithVector(curRes.GetAngleSetVec());
          if (res.Empty())
            return AngleSet();
        }
      }
    }

    return res;
	}


private:
	double distanceBetweenObstacles;
	Raft raft;
	std::vector<Segment> obstacles;
  Graph<AngleGraphElem> graph;
};