#ifndef ELEMENTLISTWIDGET_H
#define ELEMENTLISTWIDGET_H

#include <vector>

#include <QWidget>
#include <QDoubleSpinBox>
#include <QFormLayout>

class ElementListWidget : public QWidget {
    Q_OBJECT

private:
    std::vector<QWidget *> m_list;

    QFormLayout *m_ly;

public:
    explicit ElementListWidget(QWidget *parent = 0);

    void addWidget(QWidget *qw);

signals:

public slots:
};

#endif // ELEMENTLISTWIDGET_H
