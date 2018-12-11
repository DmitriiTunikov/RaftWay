/* Lebed Pavel, IPMM, 23631\2, 27.02.2018 */

#ifndef __ANIM_H_INCLUDED_
#define __ANIM_H_INCLUDED_
#pragma once

#include "vector2d.h"
#include "segment.h"
#include "colordef.h"
#include <string>
//#include <glut.h>
#include "angle.h"

#pragma comment(lib, "glut32.lib")


class Anim2d
{
public:
  static void Start( void );
  static void End(void);

  static void DrawPoint( const Vector2d &Point, const color_t &Color );
 
  static void DrawSegment( const Segment &Seg, const color_t &Color, float LineWidth = 0.001 );

  static void DrawStr( const std::string &Str, const color_t &Color,
                       const Vector2d &Pos );

  static void DrawAngle( const Vector2d &Center, const Angle &An, const color_t &Color, double Delta );
};

#endif /* __ANIM_H_INCLUDED_ */

/* END OF 'anim2d.h' FILE */

