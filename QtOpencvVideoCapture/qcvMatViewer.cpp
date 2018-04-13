#include <opencv2/imgproc/imgproc.hpp>

#include "qcvMatViewer.h"

using cv::Mat;

QCVMatViewer::QCVMatViewer(QWidget *parent) :
    QWidget(parent), m_height(0), m_width(0) {
}

QCVMatViewer::~QCVMatViewer() {
}

void QCVMatViewer::resizeEvent(QResizeEvent *e) {
    QWidget::resizeEvent(e);
    m_height = height();
    m_width = width();

    paintImage(m_imgOriginal);

    repaint();
}

void QCVMatViewer::paintEvent(QPaintEvent* /*event*/) {
    // Display the image
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), m_qimage);
    painter.end();
}

void QCVMatViewer::showImage(const cv::Mat& image) {
    // Convert the image to the RGB888 format
    switch (image.type()) {
    case CV_8UC1:
        cvtColor(image, m_imgOriginal, CV_GRAY2RGB);
        break;
    case CV_8UC3:
        cvtColor(image, m_imgOriginal, CV_BGR2RGB);
        break;
    case CV_8UC4:
        cvtColor(image, m_imgOriginal, CV_RGBA2RGB);
        break;
    }

    // QImage needs the data to be stored continuously in memory
    assert(m_imgOriginal.isContinuous());

    paintImage(m_imgOriginal);

    repaint();
}

void QCVMatViewer::paintImage(const Mat &img) {
    // Generate an image with the canvas size to store the image to be
    // represented inside.
    m_imgCanvas = Mat::zeros(m_height, m_width, CV_8UC3);

    if (!img.empty()) {
        // Compute the aspect ratio of the image and the canvas
        cv::Size size;
        cv::Rect inRect;
        float iAsp = img.cols / static_cast<float>(img.rows);
        float cAsp = m_width / static_cast<float>(m_height);
        if (iAsp < cAsp) {
            // Compute the size of the inner image
            int w = static_cast<int>(m_height * iAsp);
            size = cv::Size(w, m_height);
            // Compute the coordinate rect of the inner image
            int offset = (m_width - w) / 2;
            inRect = cv::Rect(offset, 0, w, m_height);
        } else {
            // Compute the size of the inner image
            int h = static_cast<int>(m_width / iAsp);
            size = cv::Size(m_width, h);
            // Compute the coordinate rect of the inner image
            int offset = (m_height - h) / 2;
            inRect = cv::Rect(0, offset, m_width, h);
        }

        cv::Mat croppedImg = m_imgCanvas(inRect);
        // Resize the image to fit in the current canvas
        cv::resize(img, croppedImg, size);
    }

    // Assign OpenCV's image buffer to the QImage. Note that the bytesPerLine
    // parameter (http://qt-project.org/doc/qt-4.8/qimage.html#QImage-6) is
    // 3 * width because each pixel has three bytes.
    m_qimage = QImage(m_imgCanvas.data, m_imgCanvas.cols, m_imgCanvas.rows,
                      m_imgCanvas.cols*3, QImage::Format_RGB888);
}
