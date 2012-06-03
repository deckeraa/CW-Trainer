/************************************************************************
 *  THIS CODE IS RELEASED UNDER THE
 *  GNU GENERAL PUBLIC LICENSE
 *  Version 2, June 1991
 *
 *  See LICENSE for more information
 *
 ************************************************************************/

#ifndef __MY__LAYOUT__H__
#define __MY__LAYOUT__H__

#include <qwidget.h>
#include <qtimer.h>
#include <qbuttongroup.h>
#include <qtextedit.h>
//#include "lcdrange.h"
#include <QLCDNumber>
#include "cwsound.h"

class MyWidget : public QWidget
{
  Q_OBJECT

private:
  CWSoundMachine* m_cw;
  QTimer *m_timer;
  QButtonGroup *m_lettergroup;
  QTextEdit* m_cwtext;
  QLCDNumber* m_wpm;
  QLCDNumber* m_spacing;

  QString m_charpipe;

  //chars in suggest Koch order
  static const char * m_cwchars;
  char m_charsInTest[41];
  int m_numInTest;

  char m_lastchar;
  int m_numingroup;
  int m_charspace;
  int m_wordspace;
public:
  MyWidget( QWidget *parent=0, const char *name=0 );
  ~MyWidget()
    {
      delete m_cw;
      delete m_timer;
      delete m_lettergroup;
      delete m_cwtext;
    }

protected slots:
    void StartCW();
    void EndCW();
    void About();
    void ClearAll();
    void SelectAll();
    void NewSpacing(int);
    void NewSpeed(int);
    void ReadFile();

    void playNextChar();
};

#endif
