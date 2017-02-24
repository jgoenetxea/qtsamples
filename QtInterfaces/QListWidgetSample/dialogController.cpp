#include "dialogController.h"

#include <QToolButton>
#include <QFormLayout>


DialogController::DialogController(QWidget *parent) : QWidget(parent) {
    ui.setupUi(this);

    m_elemListWidget = ui.listWidget;

    int n = 10;

    m_vList.resize(n);

    for (int i = 0; i < n; ++i) {
        m_vList[i] = new CustomListItemWidget();
        m_vList[i]->init(1, 2, 3);

        QListWidgetItem *item = new QListWidgetItem();
        m_elemListWidget->addItem(item);
        m_elemListWidget->setItemWidget(item, m_vList[i]);
    }
}
