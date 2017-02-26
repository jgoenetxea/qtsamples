#include "dialogController.h"

#include <QToolButton>
#include <QFormLayout>


DialogController::DialogController(QWidget *parent) : QWidget(parent) {
    ui.setupUi(this);

    m_scroll = ui.scrollArea;

    int n = 100;

    m_vList.resize(n);

    for (int i = 0; i < n; ++i) {
        m_vList[i] = new QDoubleSpinBox();

        m_listWidget.addWidget(reinterpret_cast<QWidget*>(m_vList[i]));
    }

    m_scroll->setWidget(&m_listWidget);
}
