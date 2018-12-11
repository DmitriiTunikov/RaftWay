/* Lebed Pavel, IPMM, 23631\2, 27.02.2018 */

#include "anim.h"

void Anim2d::DrawSegment( const Segment &Seg, const color_t &Color, float LineWidth )
{
  //Vector2d p0 = Seg.GetBegin();
  //Vector2d p1 = Seg.GetEnd();
  //Vector2d perp = ((p1 - p0).GetPerp().Normalise()) * (LineWidth / 2);
  //
  //glBegin(GL_LINES);
  //   glColor3d(Color[0], Color[1], Color[2]);
  //   glVertex2d(p0.GetX(), p0.GetY());
  //   glVertex2d(p1.GetX(), p1.GetY());
  //glEnd();
} /* End of 'Anim2d::DrawSegment' function */

void Anim2d::DrawStr( const std::string &Str, const color_t &Color,
                      const Vector2d &Pos )
{
  //glColor3d(Color[0], Color[1], Color[2]);
  //glRasterPos2d(Pos.GetX(), Pos.GetY());
  //for (size_t i = 0; i < Str.size(); i++)
  //  glutBitmapCharacter(GLUT_BITMAP_8_BY_13, Str[i]);
} /* End of 'Anim2d::DrawStr' function */

void Anim2d::Start( void )
{
  // Start drawing
  glClearColor(Blue[0], Blue[1], Blue[2], 0);
  glClear(GL_COLOR_BUFFER_BIT); 
} /* End of 'Start' function */

void Anim2d::End( void )
{
  // Stop drawing
  glFlush();
  glutSwapBuffers();
} /* End of 'End' function */

void Anim2d::DrawPoint(const Vector2d & Point, const color_t & Color)
{
  glBegin(GL_POINT);
    glColor3d(Color[0], Color[1], Color[2]);
    glVertex2d(Point.GetX(), Point.GetY());
  glEnd();
} /* End of 'Anim2d::DrawPoint' function */

void Anim2d::DrawAngle(const Vector2d &Center, const Angle &An, const color_t &Color, double Delta)
{
  double epsi = 0.01;
  unsigned steps = (unsigned)(An.delta / epsi);
  
  glBegin(GL_LINE_STRIP);
  glColor3d(Color[0], Color[1], Color[2]);

  for (unsigned i = 0; i < steps; i++)
    glVertex2d(Center.GetX() + cos(epsi * i + Delta), Center.GetY() + sin(epsi * i + Delta));
  glEnd();
}

/* END OF 'anim2d.cpp' FILE */
