

/****************************************************************
**
** Implementation of LCDRange class, Qt tutorial 7
**
****************************************************************/

/*#include "lcdrange.h"

#include <qslider.h>
#include <qlcdnumber.h>

LCDRange::LCDRange( int min, int max, QWidget *parent, const char *name )
        : QVBoxLayout( parent, name )
{
    QLCDNumber *lcd  = new QLCDNumber( 2, this, "lcd"  );
    slider = new QSlider( Horizontal, this, "slider" );
    slider->setRange( min, max );
    connect( slider, SIGNAL(valueChanged(int)),
             lcd, SLOT(display(int)) );
    connect( slider, SIGNAL(valueChanged(int)),
             SIGNAL(valueChanged(int)) );
}

int LCDRange::value() const
{
    return slider->value();
}

void LCDRange::setValue( int value )
{
    slider->setValue( value );
}*/

// http://www.greyc.ensicaen.fr/ensicaen/Docs/Qt4/tutorial-t14-lcdrange-cpp.html

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

 #include <QLCDNumber>
 #include <QLabel>
 #include <QSlider>
 #include <QVBoxLayout>

 #include "lcdrange.h"

 LCDRange::LCDRange(QWidget *parent)
     : QWidget(parent)
 {
     init();
 }

 LCDRange::LCDRange(const QString &text, QWidget *parent)
     : QWidget(parent)
 {
     init();
     setText(text);
 }

 void LCDRange::init()
 {
     QLCDNumber *lcd = new QLCDNumber(2);
     lcd->setSegmentStyle(QLCDNumber::Filled);

     slider = new QSlider(Qt::Horizontal);
     slider->setRange(0, 99);
     slider->setValue(0);
     label = new QLabel;
     label->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
     label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

     connect(slider, SIGNAL(valueChanged(int)),
             lcd, SLOT(display(int)));
     connect(slider, SIGNAL(valueChanged(int)),
             this, SIGNAL(valueChanged(int)));

     QVBoxLayout *layout = new QVBoxLayout;
     layout->addWidget(lcd);
     layout->addWidget(slider);
     layout->addWidget(label);
     setLayout(layout);

     setFocusProxy(slider);
 }

 int LCDRange::value() const
 {
     return slider->value();
 }

 QString LCDRange::text() const
 {
     return label->text();
 }

 void LCDRange::setValue(int value)
 {
     slider->setValue(value);
 }

 void LCDRange::setRange(int minValue, int maxValue)
 {
     if (minValue < 0 || maxValue > 99 || minValue > maxValue) {
         qWarning("LCDRange::setRange(%d, %d)\n"
                  "\tRange must be 0..99\n"
                  "\tand minValue must not be greater than maxValue",
                  minValue, maxValue);
         return;
     }
     slider->setRange(minValue, maxValue);
 }

 void LCDRange::setText(const QString &text)
 {
     label->setText(text);
 }
