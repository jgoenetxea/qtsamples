#ifndef CUSTOMLISTITEMWIDGET_H
#define CUSTOMLISTITEMWIDGET_H

#include <QWidget>

#include <QDoubleSpinBox>

struct VectorToolBox {
    QDoubleSpinBox* x;
    QDoubleSpinBox* y;
    QDoubleSpinBox* z;
    int mapIdentifier;
    std::string elementId;
};


class CustomListItemWidget : public QWidget
{
    Q_OBJECT
protected:
    VectorToolBox vtb;

public:
    explicit CustomListItemWidget(QWidget *parent = 0);

    void init(const int x, const int y, const int z);

signals:

public slots:
};

#endif // CUSTOMLISTITEMWIDGET_H
