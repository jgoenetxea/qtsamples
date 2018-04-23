#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>

class QtFullscreenOverlayWidget : public QWidget {
    Q_OBJECT

 public:
    explicit QtFullscreenOverlayWidget(QWidget *parent = 0);

    //setWindowFlags(windowFlags()| Qt::FramelessWindowHint);

    ~QtFullscreenOverlayWidget();

    void paintEvent(QPaintEvent *e);
};

#endif // CUSTOMWIDGET_H
