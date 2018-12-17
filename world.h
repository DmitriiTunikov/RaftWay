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
		obstacles.push_back(Segment(center, center + Vector2d(1, 0) * (raftSide + 2 * triangleHeight)));

		//internal left
		obstacles.push_back(Segment(center, center - Vector2d(0, 1) * (raftSide + 2 * triangleHeight)));

    raft.InitRelativeSides();
	}

	const std::vector<Segment>& GetObstacles(void) {
		return obstacles;
	}

	
	bool ComputeWay(std::vector<WayPoint>& wayRes, double epsilon) {
    
		// raft can't be inside canal
		if (raft.raftSide + raft.triangleHeight > distanceBetweenObstacles)
			return false;

    InitGrid(epsilon);
    if (Dijkstra::GetAllWays(start, goal, graph, came_from)){
      Dijkstra::GetGoalWay(start, goal, came_from);
    }
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

		int gridXSize = (int)ceil(distX / epsilon);
		int gridYSize = (int)ceil(distY / epsilon);

		//init pos in grid
    int move = gridYSize;

    bool breakFlag = false;
		for (int j = 0; j < gridXSize; j++) {
      move = gridYSize;
			for (int i = 0; i < gridYSize; i++) {				
				// point coordinate
				Vector2d pos = Vector2d(minX + epsilon * j, (minY + epsilon * i));
        if (pos.GetY() < 0 && pos.GetX() > 0) {
          move--;
          breakFlag = true;
          continue;
        }
        //get angle set for current point
        cout << j << " " << i << endl;
        double t = clock();
				AngleSet angSet;
        CalculateAngles(pos, angSet);// empty set to fill in thread  = CalculateAngles(pos);
        cout << (clock() - t) / CLOCKS_PER_SEC << endl;
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

    // Caclculate points angles
    /*double t = clock();
    vector<thread *> Threads;
    unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
    for (unsigned i = 0; i < concurentThreadsSupported && i < graph.getElements().size(); i++)
    {
      AngleSet &angleSet = graph.getElements()[i].angleSet;
      Vector2d pos = graph.getElements()[i].getPos();
      auto calc = [this](Vector2d pos, AngleSet &res) {this->CalculateAngles(pos, res);};
      Threads.push_back(new std::thread(calc, std::ref(pos), std::ref(angleSet)));
    }

    unsigned j = concurentThreadsSupported, i = 0;
    while (j < graph.getElements().size())
    {
      for (i = 0; i < concurentThreadsSupported && i + j < graph.getElements().size(); i++)
      {
        Threads[i]->join();
        delete Threads[i];

        AngleSet &angleSet = graph.getElements()[i + j].angleSet;
        Vector2d pos = graph.getElements()[i + j].getPos();
        auto calc = [this](Vector2d pos, AngleSet &res) {this->CalculateAngles(pos, res);};
        Threads[i] = new std::thread(calc, pos, std::ref(angleSet));
      }
      j += i;
    }

    for (thread* (thr) : Threads)
    {
      thr->join();
      delete thr;
    }
    Threads.clear();

    t = (clock() - t) / CLOCKS_PER_SEC;
    cout << "Threads full time: " << t << endl;
    cout << "Points count: " << graph.getElements().size() << endl;
    cout << "Threads average time: " << t / graph.getElements().size() << endl;*/
    
    start = graph.getElements()[0];
    goal = graph.getElements()[graph.getElements().size() - 2];
	}



	void CalculateAngles(Vector2d& pos, AngleSet &res)
  {
    int i = 0;
    for (const Segment &Relative : raft.relativeSides)
    {
      for (const Segment &Obstacle : obstacles)
      {
        i++;
        AngleSet curRes;
        if (!EquationSolve(Obstacle, pos, Relative, curRes))
        {
          res.GetAngleSetVec().clear();
          return;
        }
        if (res.Empty())
          res = AngleSet(curRes);
        else
        {
          res.IntersectSetWithVector(curRes.GetAngleSetVec());
          if (res.Empty())
            return;
        }
      }
    }
	}


public:
	double distanceBetweenObstacles;
	Raft raft;
	std::vector<Segment> obstacles;
  Graph<AngleGraphElem> graph;
  AngleGraphElem start;
  AngleGraphElem goal;
  std::vector<AngleGraphElem> came_from;
};