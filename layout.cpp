/************************************************************************
 *  THIS CODE IS RELEASED UNDER THE
 *  GNU GENERAL PUBLIC LICENSE
 *  Version 2, June 1991
 *
 *  See LICENSE for more information
 *
 ************************************************************************/

#include <time.h>

#include <qpushbutton.h>
#include <qlabel.h>
//#include <qgrid.h>
#include <QGridLayout>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qcheckbox.h>
#include <qfont.h>
#include <qfiledialog.h>
#include <qfile.h>
#include <qtextstream.h>
#include "layout.h"
#include "cwsound.h"

#include "cwtest.h"

#define CODESPEED_MIN 15

const char * MyWidget::m_cwchars = "KMRSUAPTLOWI.NJEFOY,VG5/Q9ZH38B?427C1D6X";

MyWidget::MyWidget( QWidget *parent, const char *name )
  : QWidget( parent )
{
  srand(time(NULL));
  m_cw = new CWSoundMachine(20, 1, 600);
  m_lastchar = '&';

  QGridLayout *grid = new QGridLayout(this, 2, 2, 10);

  m_wpm = new LCDRange(CODESPEED_MIN, 40, this);
  m_wpm->setValue(20);

  connect(m_wpm, SIGNAL(valueChanged(int)),
	  SLOT(NewSpeed(int)) );


  QLabel *label = new QLabel ( m_wpm, "CW Character Speed", this);
  grid->addWidget(label, 0, 0);
  grid->addWidget(m_wpm, 1, 0);

  m_spacing = new LCDRange(5, 40, this);
  m_spacing->setValue(20);

  connect(m_spacing, SIGNAL(valueChanged(int)),
	  SLOT(NewSpacing(int)) );

  grid->addWidget(m_spacing, 1, 1);
  label = new QLabel (m_wpm, "Character Spacing", this);
  grid->addWidget(label, 0, 1);

  m_cwtext = new QMultiLineEdit(this);
  m_cwtext->setFont(QFont( "Courier", 18, QFont::Bold ));

  m_cwtext->setReadOnly(true);
  m_cwtext->setWordWrap(QMultiLineEdit::WidgetWidth);
  grid->addMultiCellWidget(m_cwtext, 2, 2, 0, 1);

  QPushButton *start = new QPushButton( "Start CW", this);
  QPushButton *stop = new QPushButton( "Stop CW", this);

  connect( start, SIGNAL(clicked()),
	   SLOT(StartCW()) );

  connect( stop, SIGNAL(clicked()),
	   SLOT(EndCW()) );

  grid->addWidget(start, 3, 0);
  grid->addWidget(stop, 3, 1);

  const char *ptr = m_cwchars;
  m_lettergroup = new QButtonGroup(5, Horizontal, "Chars", this) ;
  for(int i = 0 ; ptr[i] ; i++)
    {
      QCheckBox *cb = new QCheckBox(QString(QChar(ptr[i])), m_lettergroup);
      cb->setChecked(CWTest::CharInUse(ptr[i]));
      m_lettergroup->insert(cb);
    }

  grid->addWidget(m_lettergroup, 4, 0);

  QButtonGroup *bg = new QButtonGroup(1, Horizontal, "Actions", this) ;
  QPushButton *clearall = new QPushButton( "Clear All", bg);
  connect( clearall, SIGNAL(clicked()),
	   SLOT(ClearAll()) );

  QPushButton *selectall = new QPushButton( "Select All", bg);
  connect( selectall, SIGNAL(clicked()),
	   SLOT(SelectAll()) );

  QPushButton *loadfile = new QPushButton( "Load File...", bg);
  connect( loadfile, SIGNAL(clicked()),
	   SLOT(ReadFile()) );


  QPushButton *about = new QPushButton( "About...", bg);
  connect( about, SIGNAL(clicked()),
	   SLOT(About()) );


  grid->addWidget(bg, 4, 1);


  m_timer = new QTimer(this, "mycwtimer");
  
  connect(m_timer, SIGNAL(timeout()),
	   SLOT(playNextChar()) );

  setCaption("CW Trainer");
}

//this function is ugly...
//too many special cases and flags
void MyWidget::playNextChar()
{
  //file mode or randchar mode?
  bool dospace = false;
  bool play_char = true;
  char next_char;

  char prosign[2];
  prosign[0] = prosign[1] = 0;
  
  if(m_charpipe.isEmpty())
    {
      int j=(int) ((float)m_numInTest*rand()/(RAND_MAX+1.0));
      if(m_numingroup > 6)
	dospace = true;
      else if(m_numingroup < 2)
	dospace = false;
      else
	dospace = (2 == (int) ((float)4*rand()/(RAND_MAX+1.0))) && m_lastchar != ' ';

      m_numingroup++;

      next_char = m_charsInTest[j];
    }
  else
    {
      QChar ch = m_charpipe[m_numingroup];
      m_numingroup++;

      if(ch == '\0')
	{
	  EndCW();
	  return;
	}

      next_char = (char)ch.upper();

      //couple of translations?
      if(next_char == '-' || next_char == ':' || next_char == '\\')
	next_char = '/'; 
      

      if(next_char == '^') //prosign
	{
	  ch = m_charpipe[m_numingroup];
	  m_numingroup++;
	  if(ch == '\0')
	    {
	      EndCW();
	      return;
	    }
	  prosign[0] = (char)ch.upper();

	  ch = m_charpipe[m_numingroup];
	  m_numingroup++;
	  if(ch == '\0')
	    {
	      EndCW();
	      return;
	    }
	  prosign[1] = (char)ch.upper();
	}

      if(next_char == ' ')
	dospace = true;

      if(CWTest::GetCW(next_char) == " ")
	{
	  play_char = false;
	}
    }

  if(dospace == false)
    {
      m_lastchar = next_char;
      m_cwtext->setText(m_cwtext->text() + QString(QChar(next_char)));

      if(play_char)
	m_cw->PlayCWNote(CWTest::GetCW(next_char).c_str());
      else if(prosign[1])
	{
	  m_cw->PlayCWNote(CWTest::GetCW(prosign[0]).c_str());
	  m_cwtext->setText(m_cwtext->text() + QString(QChar(prosign[0])));
	  m_cw->PlayCWNote(CWTest::GetCW(prosign[1]).c_str());
	  m_cwtext->setText(m_cwtext->text() + QString(QChar(prosign[1])));
	}

      m_timer->changeInterval(m_charspace);
    }
  else
    {
      if(m_charpipe.isEmpty())
	m_numingroup = 0;

      m_lastchar = ' ';
      m_cwtext->setText(m_cwtext->text() + QString(" | "));
      m_timer->changeInterval(m_wordspace);
    }
}

void MyWidget::StartCW()
{
  m_timer->stop();
  //gather the checked boxes...
  memset(m_charsInTest, 0, 41);
  m_numInTest = 0;
  for(int i = 0 ; i < 40 ; i++)
    {
      QCheckBox *b = (QCheckBox*)m_lettergroup->find(i);
      if(b->isChecked())
	{
	  m_charsInTest[m_numInTest] = m_cwchars[i];
	  m_numInTest++;
	}
    }
  if(m_numInTest)
    {
      m_numingroup = 0;
      int speed = m_wpm->value();
      int space = m_spacing->value();

      NewSpeed(speed);
      NewSpacing(space);
      m_cwtext->setText(QString(""));
      m_timer->start(500);
    }
}

void MyWidget::NewSpeed(int speed)
{
  m_cw->AdjustSpeed(speed);
}

void MyWidget::NewSpacing(int space)
{
  m_charspace = (int)((((60000.0 / (float)space) / 7.0) / 3.0));
  m_wordspace = m_charspace * 2;
}

void MyWidget::EndCW()
{
  m_timer->stop();
  m_charpipe = "";
}

void MyWidget::About()
{
  QMessageBox::information( this
		 ,"About CW Trainer"
		 ,"CW Trainer 0.1.0"
		  "\nKoch and Farnsworth adaptable Morse Code trainer."
		  "\n\nBy John Wagner, N1QO and Jacob Anawalt, KD7YKO"
		  "\nhttp://sourceforge.net/projects/cwtrainer"
		  "\nGPL - Uses code from GWFON's Koch CW Trainer"
		  "\nSee README and LICENSE for more information");
}

void MyWidget::ClearAll()
{
  for(int i = 0 ; i < 40 ; i++)
    {
      QCheckBox *b = (QCheckBox*)m_lettergroup->find(i);
      b->setChecked(false);
    }
 
}

void MyWidget::SelectAll()
{
  for(int i = 0 ; i < 40 ; i++)
    {
      QCheckBox *b = (QCheckBox*)m_lettergroup->find(i);
      b->setChecked(true);
    }
 
}

void MyWidget::ReadFile()
{
  QString s( QFileDialog::getOpenFileName( QString::null, "*", this ) );
  if ( s.isEmpty() )
    return;

  QFile f(s);
  
  if ( f.open(IO_ReadOnly) ) 
    {    // file opened successfully
      m_charpipe = "";
      QTextStream t( &f );        // use a text stream

      while ( !t.eof() )         // until end of file...
	{
	  QString s = t.readLine();       // line of text excluding '\n'
	  s += " ";
	  m_charpipe += s;
	  //printf( "%3d: %s\n", n++, (const char *)s );
	}
      f.close();
      StartCW();
    }
  else
    QMessageBox::information( this, "CW Trainer",
			      "An error occurred opening the file");

}
