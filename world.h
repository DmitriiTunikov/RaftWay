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
	World(double distanceBetweenObstacles_, double epsilon_)
		: raft(nullptr),
      distanceBetweenObstacles(distanceBetweenObstacles_), epsilon(epsilon_)
	{
		//left
		obstacles.push_back(Segment(-distanceBetweenObstacles_, -distanceBetweenObstacles_, -distanceBetweenObstacles_, distanceBetweenObstacles_));
				
		//up
		obstacles.push_back(Segment(-distanceBetweenObstacles_, distanceBetweenObstacles_, distanceBetweenObstacles_, distanceBetweenObstacles_));

		//inetrnal up
		obstacles.push_back(Segment(0, 0, distanceBetweenObstacles_, 0));

		//internal left
		obstacles.push_back(Segment(0, -distanceBetweenObstacles_, 0, 0));

    InitGrid();
	}

  ~World()
  {
    delete raft;
  }

	const std::vector<Segment>& GetObstacles(void) {
		return obstacles;
	}
	
	bool ComputeWay(std::vector<WayPoint>& wayRes) {
    
    if (startId == -1 || endId == -1)
      return false;
		// raft can't be inside canal
		if (raft->raftSide + raft->triangleHeight > distanceBetweenObstacles)
			return false;

    //InitGrid();
    //double t = clock();
    if (Dijkstra::GetAllWays(start, goal, graph, came_from)){
      wayRes = Dijkstra::GetGoalWay(start, goal, came_from);
      return true;
    }
    //cout << "Graph serach: " << (clock() - t) / CLOCKS_PER_SEC << endl;
		//return false;
	}

  void UpdateWorld(double raftSide, double trangleSide, double halfCircleRadius){
    //update raft sides
    UpdateRaft(Vector2d(0, 0), Vector2d(0, 1), raftSide, trangleSide, halfCircleRadius);

    //update grid
    minX = -distanceBetweenObstacles + raft->raftSide / 2;
    maxX = raft->raftSide / 2 + raft->triangleHeight;
    minY = -distanceBetweenObstacles + raft->raftSide / 2;
    maxY = distanceBetweenObstacles - raft->raftSide / 2;
    downMaxX = -raft->raftSide / 2;

    int offsetStart = (raftSide / 2 / epsilon) * gridYSize + 1;
    int offsetEnd = (raftSide / 2 / epsilon);

    double distX = maxX - minX;
    double distY = maxY - minY;


    startId = -1;
    endId = -1;

    CalculateAnglesAll();
    //CalculateAnglesAll();
    //init start and goal
    if (startId != -1 && endId != -1)
    {
      start = graph.getElements()[startId];
      goal = graph.getElements()[endId];
    }
  }

private:
  void CalculateAnglesAll()
  {
    vector<thread *> Threads;
    vector<int> Ids;
    unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
    unsigned curThread = 0;
    unsigned i;
    for (i = 0; curThread < concurentThreadsSupported && i < graph.getElements().size(); i++)
    {
      AngleGraphElem &elem = graph.getElements()[i];
      elem.angleSet.Clear();
      if (!IsPossiblePoint(elem.getPos()))
        continue;
      Vector2d pos = elem.getPos();
      auto calc = [this](Vector2d pos, AngleSet &res) {this->CalculateAngles(pos, res);};
      Ids.push_back(i);
      Threads.push_back(new std::thread(calc, pos, std::ref(elem.angleSet)));
      curThread++;
    }

    unsigned j = i;
    while (j < graph.getElements().size())
    {
      curThread = 0;
      for (; curThread < Threads.size() && j < graph.getElements().size(); j++)
      {
        AngleGraphElem &elem = graph.getElements()[j];
        elem.angleSet.Clear();
        if (!IsPossiblePoint(elem.getPos()))
          continue;
        thread* th = Threads[curThread];
        th->join();

        // check can be start
        AngleGraphElem &elemCalculated = graph.getElements()[Ids[curThread]];
        if (startId == -1 && abs(elemCalculated.getPos().GetY() - minY) <= epsilon && !elemCalculated.angleSet.Empty())
        {
          startId = Ids[curThread];
        }
        else if (endId == -1 && abs(elemCalculated.getPos().GetX() - maxX) <= epsilon && !elemCalculated.angleSet.Empty())
        {
          endId = Ids[curThread];
        }

        th->~thread();

        Vector2d pos = elem.getPos();
        auto calc = [this](Vector2d pos, AngleSet &res) {this->CalculateAngles(pos, res);};
        Threads[curThread] = new (th) thread(calc, pos, std::ref(elem.angleSet));
        Ids[curThread] = j;
        curThread++;
      }
    }

    for (thread* (thr) : Threads)
    {
      thr->join();
      // check can be start
      AngleGraphElem &elemCalculated = graph.getElements()[Ids[curThread]];
      if (startId == -1 && abs(elemCalculated.getPos().GetY() - minY) <= epsilon && !elemCalculated.angleSet.Empty())
      {
        startId = Ids[curThread];
      }
      else if (endId == -1 && abs(elemCalculated.getPos().GetX() - maxX) <= epsilon && !elemCalculated.angleSet.Empty())
      {
        endId = Ids[curThread];
      }
      delete thr;
    }
    Threads.clear();
  }

  void CalculateAnglesAllDebug()
  {
    for (unsigned i = 0; i < graph.getElements().size(); i++)
    {
      AngleGraphElem &elem = graph.getElements()[i];
      elem.angleSet.Clear();
      if (!IsPossiblePoint(elem.getPos()))
        continue;
      CalculateAngles(elem.getPos(), elem.angleSet);
      if (startId == -1 && abs(elem.getPos().GetY() - minY) <= epsilon && !elem.angleSet.Empty())
      {
        startId = i;
      }
      else if (endId == -1 && abs(elem.getPos().GetX() - maxX) <= 2 * epsilon && !elem.angleSet.Empty())
      {
        endId = i;
      }
    }
  }


  void UpdateRaft(Vector2d &raftCenter_, Vector2d &front_, double raftSide_, double triangleSide_, double halfCircleRadius_)
  {
    delete raft;
    raft = new Raft(raftCenter_, front_, raftSide_, triangleSide_, halfCircleRadius_);
  }

  bool IsPossiblePoint( const Vector2d &Point ) const
  {
    double x = Point.GetX(), y = Point.GetY();

    if (x < minX || x > maxX || y < minY || y > maxY)
      return false;
    return !(y < 0 && x > downMaxX) && !(x > 0 && y < raft->raftSide / 2);
  }

	void InitGrid() {
		minX = -distanceBetweenObstacles;
		maxX = 2 * distanceBetweenObstacles;
		minY = -distanceBetweenObstacles;
		maxY = distanceBetweenObstacles;

		double distX = maxX - minX;
		double distY = maxY - minY;

		gridXSize = (int)ceil(distX / epsilon);
		gridYSize = (int)ceil(distY / epsilon);

		//init pos in grid
    int move = gridYSize;

    double t = clock();

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
        //cout << j << " " << i << endl;
        //double t = clock();
        //CalculateAngles(pos, angSet);// empty set to fill in thread  = CalculateAngles(pos);
        //cout << (clock() - t) / CLOCKS_PER_SEC << endl;
        //add elem to graph
        graph.addElem(AngleGraphElem(pos, AngleSet()));

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
    //cout << "Create graph: " << (clock() - t) / CLOCKS_PER_SEC << endl;
	}

	void CalculateAngles(const Vector2d& pos, AngleSet &res)
  {
    int i = 0;
    for (const Segment &Relative : raft->relativeSides)
    {

      i++;
      for (const Segment &Obstacle : obstacles)
      {
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
	double distanceBetweenObstacles, minX, minY, maxX, maxY, downMaxX;
  int gridXSize, gridYSize, startId, endId;
	Raft *raft;
  double epsilon;
	std::vector<Segment> obstacles;
  Graph<AngleGraphElem> graph;
  AngleGraphElem start;
  AngleGraphElem goal;
  std::vector<CameFromElem> came_from;
};