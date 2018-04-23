#include "QtFullscreenOverlayWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtFullscreenOverlayWidget w;
    w.show();

    return a.exec();
}
