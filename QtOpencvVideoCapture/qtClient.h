#ifndef QTCLIENT_H
#define QTCLIENT_H

#include "qcvMatViewer.h"

#include <QTimer>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class QtClient : public QCVMatViewer {
    Q_OBJECT

 public:
    QtClient(QWidget *parent = 0);
    ~QtClient();

 private
    slots:
    void processNextFrame();

 private:
    void renderImage(const cv::Mat& _img);
    bool initCameraCapture(const std::string& _source = "");
    bool getNewImage(cv::Mat* _img);
    void clear();

    cv::Mat m_colorImg, m_grayImg;
    cv::VideoCapture m_capture;

    int m_width;
    int m_height;
    int m_fps;
    int m_fourcc ;
    int m_frameCount;

    QTimer *m_timer;
};

#endif // QTCLIENT_H
