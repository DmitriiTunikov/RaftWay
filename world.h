#pragma once

#include <vector>

#include "segment.h"
#include "raft.h"
#include "dijkstra.h"
#include "angle_graph_elem.h"
#include "graph.h"

using AngleInterval = std::pair<double, double>;

class World {
public:
	World(double distanceBetweenObstacles_, Vector2d raftCenter, Vector2d front, double raftSide, double triangleSide, double halfCircleRadius)
		: raft(raftCenter, front, raftSide, triangleSide, halfCircleRadius)
		, distanceBetweenObstacles(distanceBetweenObstacles_)
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
		/*double minX = -distanceBetweenObstacles + raft.raftSide / 2;
		double maxX = raft.raftSide / 2 + raft.triangleHeight;
		double minY = -raft.raftSide / 2 - raft.triangleHeight;
		double maxY = distanceBetweenObstacles - raft.raftSide / 2;

		double distX = maxX - minX;
		double distY = maxY - minY;

		int gridXSize = distX / epsilon;
		int gridYSize = distY / epsilon;

		// reserve memory
		for (double i = 0; i < gridYSize; i++) {
			gridWorld.reserve(gridXSize);
		}


		//init pos in grid
		for (double j = 0; j < gridXSize; j++) {
			for (double i = 0; i < gridYSize; i++) {				
				// point coordinate
				gridWorld[i][j].pos = Vector2d(minX + distX * i, (minY + distY * j));
				gridWorld[i][j].isBlocked = false;
				if (gridWorld[i][j].pos.GetY() < 0 && gridWorld[i][j].pos.GetX() > 0) {
					break;
				}	
				CalculateAngles(gridWorld[i][j]);
			}
		}*/
	}

	void CalculateAngles(AngleGraphElem& vert) {
		
	}


private:
	double distanceBetweenObstacles;
	Raft raft;
	std::vector<Segment> obstacles;

	std::vector<AngleGraphElem> gridWorld;
};