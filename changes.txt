[CWTrainer](http://sourceforge.net/projects/cwtrainer/)
"layout.h:16:28: fatal error: qmultilineedit.h: No such file or directory"
[QMultiLineEdit](http://www.vision.ee.ethz.ch/computing/sepp-irix/qt-3.0-mo/qmultilineedit.html)
is now obsolete, so line 16 of layout.h was changed from
#include <qmultilineedit.h>
to
#include <qtextedit.h>


lcdrange.h:12:19: fatal error: qvbox.h: No such file or directory
Lines 30 and 31 changed from
LCDRange* m_wpm;
LCDRange* m_spacing;
to
QLCDNumber* m_wpm;
QLCDNumber* m_spacing;

main.cpp:30:5: error: 'class QApplication' has no member named 'setMainWidget'

````a.setMainWidget( &w );``` in main.cpp
deleted as per [this site](main.cpp:30:5: error: 'class QApplication' has no
member named 'setMainWidget').


layout.cpp:14:19: fatal error: qgrid.h: No such file or directory
````#include <qgrid.h>```` deleted from layout.cpp
````#include <QGridLayout>```` used instead.

The constructor for QGridLayout is incorrect. What is the meaning of 2, 2, and
10, the arguments after the widget being passed?
It looks like this system was built on Qt 3.3, so a quick look at
the [old documentation](http://doc.qt.nokia.com/3.3/qgridlayout.html) shows
that TODO

The QPushButtons in layout.cpp were recieving the QButtonGroup as their parent
widget in their constructors, which was throwing an error.
I removed that and called the addButton of the QButtonGroup to add each of
them.

I downloaded the latest version of the [LCDRange example](http://www.greyc.ensicaen.fr/ensicaen/Docs/Qt4/tutorial-t14.html)
, which solved the problems withthat class.
