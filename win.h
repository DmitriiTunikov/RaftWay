/* Lebed Pavel, IPMM, 23631\2, 27.02.2018 */

#ifndef __WIN_H_INCLUDED_
#define __WIN_H_INCLUDED_
#pragma once

#include <string>

//#include <glut.h>

#pragma comment(lib, "glut32.lib")

namespace win
{
  // Usin types definition
  using string = std::string;

  class Win
  {
    private:
      /* Window main title */
      string _title;

      int _width, _height; // Size

      int _menuId;
      /* Window constructor function */
      Win( int *Argc, char **Argv, const string &Title );

      void (*_mouseFunc)( int, int, float, float );

      void(*_activeMouseFunc)( float, float );

      void(*_procMenu)( int );

      /* Window distructor function */
      ~Win( void );

      Win( const Win &Win ) = delete;
      void operator=( const Win &Win ) = delete;

      static void Reshape( int Width, int Height );

    public:
      static void Create( int *Argc, char **Argv, const string &Title );
      static Win & GetInstance();
      static void WinDestroy();
      static void Run( void );
      static void DisplayFuncSet( void Draw( void ));
      static void IdleFuncSet( void Idle( void ));
      static void SpecialKeyFuncSet( void Keyboard( int, int, int ) );
      static void MouseFuncSet( void MouseFunc( int, int, float, float ) );
      static void ActiveMouseFuncSet(void ActiveMouseFunc(float, float));
      static void MenuFuncSet(void MenuFunc(int));

      static int WidthGet( void );
      static int HeightGet( void );

      static void Mouse( int Button, int State, int X, int Y );

      static void ActiveMouse( int X, int Y );

      static void MenuFunc( int Id );

      static void AddMenu( const string &Name, int Id );

      static void AttachMenu( int To );
    }; /* end of 'window' class */
}

#endif /* __WIN_H_INCLUDED_ */

/* END OF 'win.h' FILE */

