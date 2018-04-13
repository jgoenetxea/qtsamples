#include "qtClient.h"

#ifdef __linux__
#include <stdio.h>
#include <sys/time.h>
#include <sys/stat.h>
#endif

#include <iostream>

using std::string;
using std::vector;
using std::cout;
using std::endl;

QtClient::QtClient(QWidget *parent)
    : QCVMatViewer(parent),
    m_timer(nullptr) {
    // Init the camera capture
    if (!initCameraCapture()) {
        std::cout << "Error loading camera capture." << std::endl;
    }

    // Start the timer
    // Init the system
    // Timer
    m_timer = new QTimer();
    QObject::connect(m_timer, &QTimer::timeout,
                     this, &QtClient::processNextFrame);
    int period = round(1000./m_fps);
    m_timer->start(period);
}

QtClient::~QtClient() {

}

void QtClient::processNextFrame() {
    if (!getNewImage(&m_colorImg)) {
        std::cout << "Wrong image!!!" << std::endl;
    }

    // process image
    renderImage(m_colorImg);
}

void QtClient::renderImage(const cv::Mat& _img) {
    showImage(_img);
}

bool QtClient::initCameraCapture(const std::string& _source) {
    bool openCamera = _source.size() == 0;
#ifdef __linux__
    // Check if video file exists. If dont, open the camera
    if (_source.size() > 0) {
        struct stat buffer;
        bool fileExists = (stat(_source.c_str(), &buffer) == 0);
        openCamera = !fileExists;
    }
#endif

    if (openCamera) {
        m_capture.open(0);
    } else {
        m_capture.open(_source);
    }

    m_width = 0;
    m_height = 0;
    m_fps = 0;
    m_fourcc = 0;
    m_frameCount = 0;
    if (!m_capture.isOpened()) {
        throw std::runtime_error("Video can not be opened");
        return false;
    } else {
        // Show video information
        m_width = static_cast<int>(m_capture.get(CV_CAP_PROP_FRAME_WIDTH));
        m_height = static_cast<int>(m_capture.get(CV_CAP_PROP_FRAME_HEIGHT));
        m_fps = static_cast<int>(m_capture.get(CV_CAP_PROP_FPS));
        m_fourcc = static_cast<int>(m_capture.get(CV_CAP_PROP_FOURCC));
        if (!openCamera) {
            m_frameCount =
                      static_cast<int>(m_capture.get(CV_CAP_PROP_FRAME_COUNT));
        }

        cout << "Input video: (" << m_width  << " x " << m_height << ") at "
             << m_fps << " fps, fourcc = " << m_fourcc << ", numFrames = "
             << m_frameCount << "\n" << endl;
    }

    return true;
}

bool QtClient::getNewImage(cv::Mat* _img) {
    // Get current image
    m_capture >> (*_img);

    if (_img->empty()) {
        return false;
    }

    return true;
}

void QtClient::clear() {
    m_capture.release();
    m_width = 0;
    m_height = 0;
    m_fps = 0;
    m_fourcc = 0;
    m_frameCount = 0;
}
