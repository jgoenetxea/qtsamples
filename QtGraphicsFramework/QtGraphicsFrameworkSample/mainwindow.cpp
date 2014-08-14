#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphicsview.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    m_view = new GraphicsView(m_scene);

//    m_view->setRenderHint(QPainter::Antialiasing);
//    m_view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
//    m_view->setBackgroundBrush(QColor(230, 200, 167));

//    ui->graphicsView = m_view;

    ui->graphicsView->setScene(m_scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}
