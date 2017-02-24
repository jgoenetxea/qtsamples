#ifndef DIALOGCONTROLLER_H
#define DIALOGCONTROLLER_H

#include <QWidget>
#include <QDoubleSpinBox>

#include "ui_maindialog.h"

#include "customListItemWidget.h"

// Following things in: http://www.qtcentre.org/threads/27777-Customize-QListWidgetItem-how-to


class DialogController : public QWidget
{
    Q_OBJECT

private:
    Ui::Form ui;
    QListWidget *m_elemListWidget;

    std::vector<CustomListItemWidget*> m_vList;

public:
    DialogController(QWidget *parent = 0);

signals:

public slots:
};

#endif // DIALOGCONTROLLER_H
