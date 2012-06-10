

/****************************************************************
**
** Definition of LCDRange class, Qt tutorial 7
**
****************************************************************/

#ifndef LCDRANGE_H
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

#endif // LCDRANGE_H
