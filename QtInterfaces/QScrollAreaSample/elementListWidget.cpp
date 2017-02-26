#include "elementListWidget.h"

ElementListWidget::ElementListWidget(QWidget *parent) : QWidget(parent) {
    m_ly = new QFormLayout;
    this->setLayout(m_ly);
}

void ElementListWidget::addWidget(QWidget *qw) {
    m_list.push_back(qw);

    m_ly->addRow("Number:", qw);
}
