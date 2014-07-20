#include "glopenglhandler.h"

GLOpenglHandler::GLOpenglHandler(QWidget *parent) :
    QGLWidget(parent)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start(16);
}


void GLOpenglHandler::initializeGL()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1);
}

void GLOpenglHandler::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
        glColor3f(1,0,0);
        glVertex3f(-1, -0.5, 0);
        glColor3f(0,1,0);
        glVertex3f( 0.0, -0.5, 0);
        glColor3f(0,0,1);
        glVertex3f(-0.5,  0.5, 0);
    glEnd();
}

void GLOpenglHandler::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}


