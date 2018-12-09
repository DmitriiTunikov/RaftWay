#pragma once

#define _USE_MATH_DEFINES
#include <vector>
#include <math.h>

#include "geometry.h"
#include "segment.h"
#include "half_circle.h"
#include "vector2d.h"

class Raft {
public:
	Raft(Vector2d &raftCenter_, Vector2d &front_, double raftSide_, double triangleSide_, double halfCircleRadius_) 
		: front(front_),
    raftCenter(raftCenter_),
    raftSide(raftSide_),
    triangleSide(triangleSide_),
    halfCircleRadius(halfCircleRadius_)
	{

		Vector2d unitRaftSideX = front.Normalise().Rotate(-M_PI / 2);
		Vector2d unitRaftSideY = front.Normalise();

		Geometry* leftSide = new Segment(raftCenter + unitRaftSideX * (-raftSide / 2) + unitRaftSideY * (raftSide / 2), 
										raftCenter + unitRaftSideX * (-raftSide / 2) + unitRaftSideY * (-raftSide / 2));
		
		Geometry* rightSide = new Segment(raftCenter + unitRaftSideX * (raftSide / 2) + unitRaftSideY * (raftSide / 2),
										 raftCenter + unitRaftSideX * (raftSide / 2) + unitRaftSideY * (-raftSide / 2));


		Geometry* downLeftSide = new Segment(raftCenter + unitRaftSideX * (-raftSide / 2) + unitRaftSideY * (-raftSide / 2),
						         		    raftCenter + unitRaftSideX * (-raftSide / 2) + unitRaftSideY * (-raftSide / 2) + unitRaftSideX * (raftSide / 2 - halfCircleRadius));

		
		Geometry* downRightSide = new Segment(raftCenter + unitRaftSideX * (raftSide / 2) + unitRaftSideY * (-raftSide / 2),
											 raftCenter + unitRaftSideX * (raftSide / 2) + unitRaftSideY * (-raftSide / 2) + unitRaftSideX * (-(raftSide / 2 - halfCircleRadius)));

		
		Geometry* upLeftSide = new Segment(raftCenter + unitRaftSideX * (-raftSide / 2) + unitRaftSideY * (raftSide / 2),
							    		  raftCenter + unitRaftSideX * (-raftSide / 2) + unitRaftSideY * (raftSide / 2) + unitRaftSideX * (raftSide / 2 - triangleSide / 2));

		Geometry* upRightSide = new Segment(raftCenter + unitRaftSideX * (raftSide / 2) + unitRaftSideY * (raftSide / 2),
										   raftCenter + unitRaftSideX * (raftSide / 2) + unitRaftSideY * (raftSide / 2) + unitRaftSideX * (-(raftSide / 2 - triangleSide / 2)));

		
		triangleHeight = triangleSide * sqrt(3) / 2;
		Geometry* leftTriangleSide = new Segment(raftCenter + unitRaftSideX * (-raftSide / 2) + unitRaftSideY * (raftSide / 2) + unitRaftSideX * (raftSide / 2 - triangleSide / 2),
												raftCenter + unitRaftSideY * (raftSide / 2 + triangleHeight));


		Geometry* rightTriangleSide = new Segment(raftCenter + unitRaftSideX * (raftSide / 2) + unitRaftSideY * (raftSide / 2) + unitRaftSideX * (-(raftSide / 2 - triangleSide / 2)),
												 raftCenter + unitRaftSideY * (raftSide / 2 + triangleHeight));

		Vector2d halfCircleCenter(raftCenter + unitRaftSideY * (-raftSide / 2));
		Geometry* halfCircle = new HalfCircle(halfCircleCenter, halfCircleRadius, front);


		sides.push_back(leftSide);
		sides.push_back(rightSide);
		sides.push_back(downLeftSide);
		sides.push_back(downRightSide);
		sides.push_back(upLeftSide);
		sides.push_back(upRightSide);

		sides.push_back(leftTriangleSide);
		sides.push_back(rightTriangleSide);

		sides.push_back(halfCircle);
	}

	void MoveParallel(const Vector2d& moveVec) {
		for (auto raftPart : sides) {
			raftPart->MoveParallel(moveVec);
		}
	}

	void Rotate(double angle) {
		for (auto raftPart : sides) {
			raftPart->Rotate(angle, /**/raftCenter);
		}
	}

  void InitRelativeSides(void) {
    Raft raft(Vector2d(0, 0), Vector2d(0, 1), raftSide, triangleSide, halfCircleRadius);
    for (unsigned int i = 0; i < sides.size() - 1; i++) {
      relativeSides.push_back(*(Segment *)raft.sides[i]);
    }
  }

	~Raft(void) {
		for (unsigned int i = 0; i < sides.size(); i++) {
			delete sides[i];
		}
	}

public:
	double raftSide;
	double triangleHeight;
  double triangleSide;
  double halfCircleRadius;
	Vector2d front;
	Vector2d raftCenter;

  std::vector<Segment> relativeSides;
	std::vector<Geometry *> sides;
};