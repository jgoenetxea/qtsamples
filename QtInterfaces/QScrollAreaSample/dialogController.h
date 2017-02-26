#ifndef DIALOGCONTROLLER_H
#define DIALOGCONTROLLER_H

#include <QWidget>
#include <QDoubleSpinBox>

#include "ui_maindialog.h"
#include "elementListWidget.h"

class DialogController : public QWidget
{
    Q_OBJECT

private:
    Ui::Form ui;
    QScrollArea *m_scroll;
    ElementListWidget m_listWidget;

    std::vector<QDoubleSpinBox *> m_vList;

public:
    DialogController(QWidget *parent = 0);

signals:

public slots:
};

#endif // DIALOGCONTROLLER_H
