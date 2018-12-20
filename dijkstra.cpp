/*
* Motion planning problem project.
*
* FILE: AngleGraphElem.h
* AUTHORS:
*   Tunikov Dmitrii
* LAST UPDATE: 19.05.2018
* NOTE: vectex class declaration file
*/
#include <queue>
#include <cmath>
#include "dijkstra.h"
#include "priority_queue.h"
#include <map>
#include <stack>

double heuristic(const Vector2d &a, const Vector2d &b)
{
  //Manhattan distance on a square grid
  return fabs(a.GetX() - b.GetX()) + fabs(a.GetY()- b.GetY());
}

std::vector<WayPoint> Dijkstra::GetGoalWay(AngleGraphElem& start, AngleGraphElem& goal, std::vector<CameFromElem> &came_from)
{
  AngleGraphElem current = goal;
  std::vector<WayPoint> path;
  path.push_back(WayPoint(current.getPos(), current.curRealAngle)); 

  while (!(current.getPos() == start.getPos()))
  {
    int id = current.getId();
    current = came_from[current.getId()].curElems.top();
    came_from[id].curElems.pop();
    path.push_back(WayPoint(current.getPos(), current.curRealAngle));
  }
  //path.push_back(start.getPos());
  std::reverse(path.begin(), path.end());
  return path;
} /* End of 'GetGoalWay' function */

bool Dijkstra::GetAllWays(AngleGraphElem& start, AngleGraphElem& goal, Graph<AngleGraphElem>& graph,
  std::vector<CameFromElem> &came_from)
{
  //init vectors
  PriorityQueue<AngleGraphElem, double> frontier;

  //add start point to frontier with different angles
  for (Angle& a : start.getAngleSet().GetAngleSetVec()){
    AngleGraphElem startNew;
    startNew = start;
    startNew.setCurAngle(a);
    startNew.setCurRealAngle(startNew.getCurAngle());
    frontier.push(startNew, 0);
  }

  std::vector<AngleGraphElem> graph_vert = graph.getElements();
  came_from = std::vector<CameFromElem>(graph_vert.size());
  std::vector<double> cost_so_far(graph_vert.size());
  
  //came_from[start.getId()].curElems.push(AngleGraphElem(Vector2d(-1, -1), AngleSet()));
  cost_so_far[start.getId()] = 0;
  
  while (!frontier.empty()) 
  {
    AngleGraphElem current = frontier.pop();

    if (current.getPos() == goal.getPos()){
      goal = current;
    //(current.getCurRealAngle() > 3. * M_PI / 2 && current.getCurRealAngle() < PI2))
      return true;
    }
    //add neighbors to frontier 
    std::vector<int> neighbors = current.getNeighbors();
    for (auto next : neighbors)
    {
      //check that vertex has angles to be
      if (graph_vert[next].getAngleSet().GetAngleSetVec().size() == 0)
        continue;
      //check cost of this move
      double new_cost = cost_so_far[current.getId()] + 1.;
      
      AngleSet angs;
      std::vector <Angle> realAngles;
      //if algorithm wasn't at this point with some angle -> try to visit this point
      if (graph_vert[next].getAngleSet().Intersected(current.getCurAngle(), angs, realAngles)){

        AngleVector angsVec = angs.GetAngleSetVec();
        
        cost_so_far[next] = new_cost;
        double priority = new_cost + heuristic(current.getPos(), graph_vert[next].getPos());
        
        //add all next elems to frontier with different current angles
        for (unsigned int i = 0; i < angsVec.size(); i++){
          //copy graph element
          AngleGraphElem newFrontElem(graph.getElements()[next].getPos(), graph.getElements()[next].getAngleSet());
          newFrontElem = graph.getElements()[next];
          
          //set new front elem cur angle
          newFrontElem.setCurAngle(angsVec[i]);
          newFrontElem.setCurRealAngle(realAngles[i]);
          
          //push it to front
          frontier.push(newFrontElem, 1.0f / priority);
        }
        std::stack<AngleGraphElem> curElemCameFrom = came_from[current.getId()].curElems;
        bool isHaveInCameFrom = false;
        while (!curElemCameFrom.empty()){
          AngleGraphElem elem = curElemCameFrom.top();
          curElemCameFrom.pop();
          if (elem.getId() == next)
            isHaveInCameFrom = true;
        }
        //fill came from vec
        if (!isHaveInCameFrom)
          came_from[next].curElems.push(current);
      }      
    }
  }
  return false;
} /* End of 'GetAllWays' function */
