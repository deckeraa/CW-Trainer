/************************************************************************
 *  THIS CODE IS RELEASED UNDER THE
 *  GNU GENERAL PUBLIC LICENSE
 *  Version 2, June 1991
 *
 *  See LICENSE for more information
 *
 ************************************************************************/

/****************************************************************
 **
 ** mtrainer
 ** cw training software
 **
 ** Based on and includes some of Ray Goff, G4FON's "Koch" for windows
 ** 
 **
 ** 
 **
 ****************************************************************/

#include <qapplication.h>
#include "layout.h"

int main( int argc, char **argv )
{
  QApplication a( argc, argv );

  MyWidget w(NULL, "CW Trainer");
//  a.setMainWidget( &w );
  w.setMinimumSize(640, 480);
  w.show();
  return a.exec();
}
