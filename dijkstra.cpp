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

float heuristic(const Vector2d &a, const Vector2d &b)
{
  //Manhattan distance on a square grid
  return fabs(a.GetX() - b.GetX()) + fabs(a.GetY()- b.GetY());
}

std::vector<WayPoint> Dijkstra::GetGoalWay(AngleGraphElem& start, AngleGraphElem& goal, std::vector<AngleGraphElem> &came_from)
{
  AngleGraphElem current = goal;
  std::vector<WayPoint> path;
  path.push_back(WayPoint(current.getPos(), (current.getCurAngle().a + current.getCurAngle().b) / 2));

  while (!(current.getPos() == start.getPos()))
  {
    current = came_from[current.getId()];
    path.push_back(WayPoint(current.getPos(), (current.getCurAngle().a + current.getCurAngle().b) / 2));
  }
  //path.push_back(start.getPos());
  std::reverse(path.begin(), path.end());
  return path;
} /* End of 'GetGoalWay' function */

bool Dijkstra::GetAllWays(AngleGraphElem& start, AngleGraphElem& goal, Graph<AngleGraphElem>& graph,
  std::vector<AngleGraphElem> &came_from)
{
  //init vectors
  PriorityQueue<AngleGraphElem, float> frontier;

  //add start point to frontier with different angles
  AngleGraphElem start1;
  start.setCurAngle(start.getAngleSet().GetAngleSetVec()[1]);
  start.curRealAngle = (start.curAngle.a + start.curAngle.b) / 2;
  start1 = start;
  start1.curAngle = start.getAngleSet().GetAngleSetVec()[0];
  start1.curRealAngle = (start1.curAngle.a + start1.curAngle.b) / 2;
  start.getAngleSet().GetAngleSetVec().clear();
  frontier.push(start, 0);
  frontier.push(start1, 0);

  std::vector<AngleGraphElem> graph_vert = graph.getElements();
  came_from = std::vector<AngleGraphElem>(graph_vert.size());
  std::vector<float> cost_so_far(graph_vert.size());
  
  came_from[start.getId()] = AngleGraphElem(Vector2d(-1, -1), AngleSet());
  cost_so_far[start.getId()] = 0;
  
  while (!frontier.empty()) 
  {
    AngleGraphElem current = frontier.pop();

    if (current.getPos() == goal.getPos())
      return true;
    //add neighbors to frontier 
    std::vector<int> neighbors = current.getNeighbors();
    for (auto next : neighbors)
    {
      //check that vertex has angles to be
      if (graph_vert[next].getAngleSet().GetAngleSetVec().size() == 0)
        continue;
      //check cost of this move
      float new_cost = cost_so_far[current.getId()] + graph.getWeight(current.getId(), next);
      
      AngleSet angs;
      std::vector <double> realAngles;
      //if algorithm wasn't at this point with some angle -> try to visit this point
      if (graph_vert[next].getAngleSet().Intersected(current.getCurAngle(), angs, realAngles)){

        AngleVector angsVec = angs.GetAngleSetVec();
        
        cost_so_far[next] = new_cost;
        float priority = new_cost + heuristic(current.getPos(), graph_vert[next].getPos());
        
        //add all next elems to frontier with different current angles
        for (int i = 0; i < angsVec.size(); i++){
          //copy graph element
          AngleGraphElem newFrontElem(graph.getElements()[next].getPos(), graph.getElements()[next].getAngleSet());
          newFrontElem = graph.getElements()[next];
          
          //set new front elem cur angle
          newFrontElem.setCurAngle(angsVec[i]);
          newFrontElem.curRealAngle = realAngles[i];
          
          //push it to front
          frontier.push(newFrontElem, 1.0f / priority);
        }
        //fill came from vec
        came_from[next] = current;
      }      
    }
  }
  return false;
} /* End of 'GetAllWays' function */
