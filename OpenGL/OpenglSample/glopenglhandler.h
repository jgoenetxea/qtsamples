#ifndef GLOPENGLHANDLER_H
#define GLOPENGLHANDLER_H

#include <QGLWidget>
#include <QTimer>

class GLOpenglHandler : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLOpenglHandler(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:
    QTimer timer;

};

#endif // GLOPENGLHANDLER_H
