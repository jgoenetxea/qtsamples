#include "QtFullscreenOverlayWidget.h"

#include <QPainter>
#include <QScreen>
#include <QGuiApplication>

QtFullscreenOverlayWidget::QtFullscreenOverlayWidget(QWidget *parent) :
    QWidget(parent) {
    bool isTrans = true;
    if (isTrans) {
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect  screenGeometry = screen->geometry();
        int height = screenGeometry.height();
        int width = screenGeometry.width() * 2;
        setGeometry(QRect(0, 0, width, height));

        // setWindowState(Qt::WindowFullScreen);  // If full screen is used
                                                  // the transparency is lost
        setWindowFlags(Qt::Widget |
                       Qt::FramelessWindowHint |         // Remove frame
                       Qt::WindowStaysOnTopHint |        // Always ontop
                       Qt::WindowTransparentForInput |   // Ignore interaction
                       Qt::X11BypassWindowManagerHint);  // Ignore toolbars
        setParent(0); // Create TopLevel-Widget
        setAttribute(Qt::WA_NoSystemBackground, true);
        setAttribute(Qt::WA_TranslucentBackground, true);
        // setAttribute(Qt::WA_PaintOnScreen); // not needed in Qt 5.2 and up
    }
}

QtFullscreenOverlayWidget::~QtFullscreenOverlayWidget() {}

void QtFullscreenOverlayWidget::paintEvent (QPaintEvent *event) {
    QPainter painter(this);
//    painter.setPen(pen);
//    painter.setBrush(brush);

    QPen pen;
    pen.setStyle(Qt::DashDotLine);
    pen.setWidth(13);
    pen.setBrush(Qt::green);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);

    QRect rect(10, 20, 180, 160);
    painter.drawRect(rect);
    painter.restore();

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}
