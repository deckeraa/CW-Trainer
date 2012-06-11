

/****************************************************************
**
** Definition of LCDRange class, Qt tutorial 7
**
****************************************************************/

/*#ifndef LCDRANGE_H
#define LCDRANGE_H

//#include <qvbox.h>
#include<QVBoxLayout>

class QSlider;

//class LCDRange : public QVBox
class LCDRange : public QVBoxLayout
{
    Q_OBJECT
public:
    LCDRange(int min, int max, QWidget *parent=0, const char *name=0 );

    int value() const;

public slots:
    void setValue( int );

signals:
    void valueChanged( int );

private:
    QSlider *slider;
};

#endif // LCDRANGE_H*/

// http://www.greyc.ensicaen.fr/ensicaen/Docs/Qt4/tutorial-t14-lcdrange-h.html

/****************************************************************************
 **
 ** Copyright (C) 2005-2006 Trolltech ASA. All rights reserved.
 **
 ** This file is part of the example classes of the Qt Toolkit.
 **
 ** This file may be used under the terms of the GNU General Public
 ** License version 2.0 as published by the Free Software Foundation
 ** and appearing in the file LICENSE.GPL included in the packaging of
 ** this file.  Please review the following information to ensure GNU
 ** General Public Licensing requirements will be met:
 ** http://www.trolltech.com/products/qt/opensource.html
 **
 ** If you are unsure which license is appropriate for your use, please
 ** review the following information:
 ** http://www.trolltech.com/products/qt/licensing.html or contact the
 ** sales department at sales@trolltech.com.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ****************************************************************************/

 #ifndef LCDRANGE_H
 #define LCDRANGE_H

 #include <QWidget>

 class QLabel;
 class QSlider;

 class LCDRange : public QWidget
 {
     Q_OBJECT

 public:
     LCDRange(QWidget *parent = 0);
     LCDRange(const QString &text, QWidget *parent = 0);

     int value() const;
     QString text() const;

 public slots:
     void setValue(int value);
     void setRange(int minValue, int maxValue);
     void setText(const QString &text);

 signals:
     void valueChanged(int newValue);

 private:
     void init();

     QSlider *slider;
     QLabel *label;
 };

 #endif
