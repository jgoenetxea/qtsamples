#include "dialogController.h"

#include <QApplication>

int main( int argc, char *argv[] )
{
    QApplication a( argc, argv );
    DialogController dc;
    dc.show();
    return a.exec();
}
