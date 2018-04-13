#ifndef QCVMATVIEWER_H_
#define QCVMATVIEWER_H_

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <opencv2/core/core.hpp>

class QCVMatViewer : public QWidget {
    Q_OBJECT

public:
    explicit QCVMatViewer(QWidget *parent = 0);
    ~QCVMatViewer();

    void showImage(const cv::Mat& image);

protected:
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent* /*event*/);

    void paintImage(const cv::Mat &img);

    QImage m_qimage;
    cv::Mat m_imgOriginal;
    cv::Mat m_imgCanvas;
    int m_width;
    int m_height;
};

#endif  // QCVMATVIEWER_H_
