#ifndef VIULIBCAMERA_H
#define VIULIBCAMERA_H

#include <opencv2/highgui/highgui.hpp>

class ViulibCamera
{
public:
    ViulibCamera();

    void init(int cameraID);
    void getFrame();
    void setParameter();
    void getParameter();

private:
    VideoCapture m_capture;
};

#endif // VIULIBCAMERA_H
