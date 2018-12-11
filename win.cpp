/* Lebed Pavel, IPMM, 23631\2, 27.02.2018 */

#include "win.h"

#include "colordef.h"

using namespace win;

// Global win
static Win *s_win;

/* Window constructor function */
Win::Win( int *Argc, char **Argv, const string &Title ) :
  _title(Title), _mouseFunc(nullptr), _activeMouseFunc(nullptr),
  _procMenu(nullptr), _menuId(glutCreateMenu(MenuFunc))
{
  glutInit(Argc, Argv); 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(700, 700);
  glutInitWindowPosition(30, 30);
  glutCreateWindow(Title.c_str());
  glClearColor(1.0, 1.0, 1.0, 1.0); 

  _width = 700;
  _height = 700;
  glViewport(0, 0, _width, _height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-2, 2, -2, 2);
  glMatrixMode(GL_MODELVIEW);
  glutReshapeFunc(Reshape);

  glutMouseFunc(Mouse);
  glutMotionFunc(ActiveMouse);

} /* End of 'Win::Win' function */

void Win::DisplayFuncSet( void Draw( void ))
{
  glutDisplayFunc(Draw);
} /* End of 'DisplayFuncSet' function */

void Win::IdleFuncSet( void Idle( void ))
{
  glutIdleFunc(Idle);
} /* End of 'IdleFuncSet' function */

void Win::SpecialKeyFuncSet(void Keyboard( int, int, int ))
{
  glutSpecialFunc(Keyboard);
} /* End of 'SpecialKeyFuncSet' function */

void Win::MouseFuncSet( void MouseFunc( int, int, float, float ) )
{
  s_win->_mouseFunc = MouseFunc;
} /* End of 'Win::MouseFuncSet' function */

void Win::ActiveMouseFuncSet(void ActiveMouseFunc(float, float))
{
  s_win->_activeMouseFunc = ActiveMouseFunc;
} /* End of 'Win::ActiveMouseFuncSet' function */

void Win::MenuFuncSet( void MenuFunc( int ) )
{
  s_win->_procMenu = MenuFunc;
} /* End of 'Win::MenuFuncSet' function */

void Win::Mouse( int Button, int State, int X, int Y )
{
  if (s_win->_mouseFunc != nullptr)
    s_win->_mouseFunc(Button, State, (float)X / s_win->_width, 1 - (float)Y / s_win->_height);
} /* End of 'Win::Mouse' function */

void Win::ActiveMouse(int X, int Y)
{
  if (s_win->_activeMouseFunc != nullptr)
    s_win->_activeMouseFunc((float)X / s_win->_width, 1 - (float)Y / s_win->_height);
} /* End of 'Win::ActiveMouse' function */

void Win::MenuFunc( int Id )
{
  if (s_win->_procMenu != nullptr)
    s_win->_procMenu(Id);
} /* End of 'Win::MenuFunc' function */

void Win::AddMenu( const string & Name, int Id )
{
  glutAddMenuEntry(Name.c_str(), Id);
} /* End of 'Win::AddMenu' function */

void Win::AttachMenu( int To )
{
  glutAttachMenu(To);
} /* End of 'Win::AttachMenu' function */

void Win::Reshape( int Width, int Height )
{
  s_win->_width = Width;
  s_win->_height = Height;

  glViewport(0, 0, Width, Height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-2, 2, -2, 2);
  glMatrixMode(GL_MODELVIEW);
} /* End of 'Win::Reshape' function */

int Win::WidthGet( void )
{
  return s_win->_width;
} /* End of 'Win::WidthGet' function */

int Win::HeightGet( void )
{
  return s_win->_height;
} /* End of 'Win::HeightGet' function */

/* Window distructor function */
Win::~Win( void )
{
} /* End of 'Win::~Win' function */

Win & Win::GetInstance()
{
  return *s_win;
} /* End of 'Win::GetInstance' function */

void Win::WinDestroy()
{
  delete s_win;
} /* End of 'WinDestroy' function */

/* Window start function (CALLBACK version) */
void Win::Run( void )
{
  glutMainLoop();
} /* End of 'Win::Run' function */

void Win::Create( int *Argc, char **Argv, const string &Title )
{
  s_win = new Win(Argc, Argv, Title);
} /* End of 'Create' function */

/* END OF 'win.cpp' FILE */
