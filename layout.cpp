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
#include "lcdrange.h"
#include <QTextEdit>
#include <QGroupBox>
#include <iostream>

#define CODESPEED_MIN 15
#define CODESPEED_MAX 40
#define CODESPEED_DEFAULT 20

#define SPACING_MIN 5
#define SPACING_MAX 40
#define SPACING_DEFAULT 20

#define CHARACTER_GRID_ROWS 8

const char * MyWidget::m_cwchars = "KMRSUAPTLOWI.NJEFOY,VG5/Q9ZH38B?427C1D6X";

MyWidget::MyWidget( QWidget *parent )
  : QWidget( parent )
{
  srand(time(NULL));
  m_cw = new CWSoundMachine(20, 1, 600);
  m_lastchar = '&';

  //QGridLayout *grid = new QGridLayout(this, 2, 2, 10);
  QGridLayout *grid = new QGridLayout(this);
  //grid->setSpacing(10);

  //m_wpm = new LCDRange(CODESPEED_MIN, 40, this);
  LCDRange *m_wpm = new LCDRange(this);
  m_wpm->setRange(CODESPEED_MIN, CODESPEED_MAX);
  //m_wpm->setValue(20);
  m_wpm->setValue(CODESPEED_DEFAULT);

  connect(m_wpm, SIGNAL(valueChanged(int)),
	  SLOT(NewSpeed(int)) );


  //QLabel *label = new QLabel ( m_wpm, "CW Character Speed", this);
  QLabel *label = new QLabel ( "CW Character Speed", this);
  //label->setBuddy(m_wpm);
  grid->addWidget(label, 0, 0);
  grid->addWidget(m_wpm, 1, 0, Qt::Alignment(Qt::AlignLeft) );

  //m_spacing = new LCDRange(5, 40, this);
  m_spacing = new LCDRange(this);
  m_spacing->setRange(SPACING_MIN, SPACING_MAX);
  //m_spacing->setValue(20);
  m_spacing->setValue(SPACING_DEFAULT);

  connect(m_spacing, SIGNAL(valueChanged(int)),
	  SLOT(NewSpacing(int)) );

  grid->addWidget(m_spacing, 1, 1);
  //label = new QLabel (m_wpm, "Character Spacing", this);
  label = new QLabel ("Character Spacing");
  grid->addWidget(label, 0, 1);

  //m_cwtext = new QMultiLineEdit(this);
  m_cwtext = new QTextEdit(this);
  m_cwtext->setFont(QFont( "Courier", 18, QFont::Bold ));

  m_cwtext->setReadOnly(true);
  //m_cwtext->setWordWrap(QMultiLineEdit::WidgetWidth);
  m_cwtext->setWordWrapMode(QTextOption::WordWrap);
  //grid->addMultiCellWidget(m_cwtext, 2, 2, 0, 1);
  grid->addWidget(m_cwtext, 2, 0, 1, 2);

  QPushButton *start = new QPushButton( "Start CW", this);
  QPushButton *stop = new QPushButton( "Stop CW", this);

  connect( start, SIGNAL(clicked()),
	   SLOT(StartCW()) );

  connect( stop, SIGNAL(clicked()),
	   SLOT(EndCW()) );

  grid->addWidget(start, 3, 0);
  grid->addWidget(stop, 3, 1);

  const char *ptr = m_cwchars;
  //m_lettergroup = new QGroupBox(5, Horizontal, "Chars", this) ;
  m_lettergroup = new QGroupBox("Chars", this) ;
  m_lettergroup->setAlignment( Qt::Alignment(Qt::AlignHCenter) );
  m_letterlayout = new QGridLayout;
  //int chars_in_row = QString(ptr).length() / CHARACTER_GRID_ROWS;
  for(int i = 0 ; ptr[i] ; i++)
    {
      //QCheckBox *cb = new QCheckBox(QString(QChar(ptr[i])), m_lettergroup);
      QCheckBox *cb = new QCheckBox(QString(QChar(ptr[i])), m_lettergroup);
      //m_lettergroup->addButton( cb );

      cb->setChecked(CWTest::CharInUse(ptr[i]));
     // guessing at coordinates.
     // QButtonGroup cannot be added to QGridLayout
     // http://www.qtcentre.org/threads/42754-Can-t-add-QButtonGroup-to-QGridLayout
     //grid->addWidget(cb , 4, i, Qt::Alignment(Qt::AlignLeft));
     //int row = 0;
     //if( i != 0) // take care of divide-by-zero
      //  row = i / chars_in_row;
     //int column  = i - row*chars_in_row;
     int row, column;
     ConvertToPosition( i, &row, &column); // modifies row and column
     m_letterlayout->addWidget(cb, row, column);

//     m_lettergroup->insert(cb);
    }
  m_lettergroup->setLayout(m_letterlayout);

  //grid->addWidget(m_lettergroup, 4, 0);
    //grid->addWidget(m_letterlayout, 4, 0);
  //grid->addLayout(m_letterlayout, 4, 0);
  grid->addWidget(m_lettergroup, 4, 0);

  //QButtonGroup *bg = new QButtonGroup(1, Horizontal, "Actions", this) ;
  //QGroupBox *bg = new QGroupBox(1, Horizontal, "Actions", this) ;
  QGroupBox *bg = new QGroupBox("Actions", this) ;
  bg->setAlignment( Qt::Alignment(Qt::AlignHCenter) );
  //QPushButton *clearall = new QPushButton( "Clear All", bg);
  QPushButton *clearall = new QPushButton( "Clear All", bg);
  connect( clearall, SIGNAL(clicked()),
	   SLOT(ClearAll()) );

  //QPushButton *selectall = new QPushButton( "Select All", bg);
  QPushButton *selectall = new QPushButton( "Select All");
  connect( selectall, SIGNAL(clicked()),
	   SLOT(SelectAll()) );

  //QPushButton *loadfile = new QPushButton( "Load File...", bg);
  QPushButton *loadfile = new QPushButton( "Load File...");
  connect( loadfile, SIGNAL(clicked()),
	   SLOT(ReadFile()) );


  //QPushButton *about = new QPushButton( "About...", bg);
  QPushButton *about = new QPushButton( "About...");
  connect( about, SIGNAL(clicked()),
	   SLOT(About()) );

  // add the QPushButtons to the QButtonGroup
  QVBoxLayout *vbuttons = new QVBoxLayout;
  //vbuttons->setSpacing(0);
  vbuttons->addWidget( clearall  );
  vbuttons->addWidget( selectall );
  vbuttons->addWidget( loadfile  );
  vbuttons->addWidget( about     );
  bg->setLayout( vbuttons );


  grid->addWidget(bg, 4, 1);
  // guessing at coordinates
  /*grid->addWidget( clearall , 4, 1);
  grid->addWidget( selectall, 4, 2);
  grid->addWidget( loadfile , 4, 3);
  grid->addWidget( about    , 4, 4);*/


  //m_timer = new QTimer(this, "mycwtimer");
  m_timer = new QTimer(this);
  
  connect(m_timer, SIGNAL(timeout()),
	   SLOT(playNextChar()) );

  //setCaption("CW Trainer");
  setWindowTitle("CW Trainer");
  setLayout( grid );
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

      //next_char = (char)ch.upper();
      next_char = ch.toUpper().toAscii();

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
	  //prosign[0] = (char)ch.upper();
	  prosign[0] = ch.toUpper().toAscii();

	  ch = m_charpipe[m_numingroup];
	  m_numingroup++;
	  if(ch == '\0')
	    {
	      EndCW();
	      return;
	    }
	  //prosign[1] = (char)ch.upper();
	  prosign[1] = ch.toUpper().toAscii();
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
      //m_cwtext->setText(m_cwtext->text() + QString(QChar(next_char)));
      m_cwtext->setText(m_cwtext->toPlainText() + QString(QChar(next_char)));

      if(play_char)
	m_cw->PlayCWNote(CWTest::GetCW(next_char).c_str());
      else if(prosign[1])
	{
	  m_cw->PlayCWNote(CWTest::GetCW(prosign[0]).c_str());
	  //m_cwtext->setText(m_cwtext->text() + QString(QChar(prosign[0])));
	  m_cwtext->setText(m_cwtext->toPlainText() + QString(QChar(prosign[0])));
	  m_cw->PlayCWNote(CWTest::GetCW(prosign[1]).c_str());
	  //m_cwtext->setText(m_cwtext->text() + QString(QChar(prosign[1])));
	  m_cwtext->setText(m_cwtext->toPlainText() + QString(QChar(prosign[1])));
	}

      //m_timer->changeInterval(m_charspace);
      m_timer->setInterval(m_charspace);
    }
  else
    {
      if(m_charpipe.isEmpty())
	m_numingroup = 0;

      m_lastchar = ' ';
      m_cwtext->setText(m_cwtext->toPlainText() + QString(" | "));
      //m_timer->changeInterval(m_wordspace);
      m_timer->setInterval(m_wordspace);
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
      if( b == NULL )
      {
         std::cerr << "Checkbox #" << i << " not found in MyWidget::SelectAll()\n";
         return;
      }
      b->setChecked(false);
    }
 
}

void MyWidget::SelectAll()
{
  for(int i = 0 ; i < 40 ; i++)
    {
      //QCheckBox *b = (QCheckBox*)m_lettergroup->find(i);
      int row, column;
      ConvertToPosition( i , &row, &column); // modifies row and column
      QCheckBox *b = (QCheckBox*)m_letterlayout->itemAtPosition(row, column);
      if( b == NULL )
      {
         std::cerr << "Checkbox #" << i << " not found in MyWidget::SelectAll()\n";
         return;
      }
      b->setChecked(true);
    }
 
}

void MyWidget::ReadFile()
{
  //QString s( QFileDialog::getOpenFileName( QString::null, "*", this ) );
  QString s( QFileDialog::getOpenFileName( this, "Open File", QString::null, "*" ) );
  if ( s.isEmpty() )
    return;

  QFile f(s);
  
  if ( f.open(QIODevice::ReadOnly) ) 
    {    // file opened successfully
      m_charpipe = "";
      QTextStream t( &f );        // use a text stream

      //while ( !t.eof() )         // until end of file...
      while ( !t.atEnd() )         // until end of file...
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

/**
* Takes a single integer and converts it to acoordinate on the grid of characters.
* The coordinate is passed back using the two reference paramaters, row and column.
*/
void MyWidget::ConvertToPosition( int i, int *row, int *column)
{
   const char *ptr = m_cwchars;
   int chars_in_row = QString(ptr).length() / CHARACTER_GRID_ROWS;
   *row = 0;
   if( i != 0) // take care of divide-by-zero
      *row = i / chars_in_row;
   *column  = i - (*row) * chars_in_row;
}

/**
* Takes a row and a column and converts it to a single index in the grid of characters.
*/
int MyWidget::ConvertFromPosition( int row, int column )
{
   const char *ptr = m_cwchars;
   int chars_in_row = QString(ptr).length() / CHARACTER_GRID_ROWS;
   return column + row * chars_in_row;
}
