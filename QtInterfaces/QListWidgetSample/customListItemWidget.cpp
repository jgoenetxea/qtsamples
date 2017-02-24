#include "customListItemWidget.h"

#include <QFrame>
#include <QToolButton>
#include <QFormLayout>

QDoubleSpinBox* generateNewSpinBox(double value,
                                   double stepSize,
                                   int minRange,
                                   int maxRange) {
    QDoubleSpinBox* dsp = new QDoubleSpinBox;
    dsp->setRange(minRange, maxRange);
    dsp->setDecimals(8);
    dsp->setSingleStep(stepSize);
    dsp->setValue(value);
    return dsp;
}

CustomListItemWidget::CustomListItemWidget(QWidget *parent) : QWidget(parent)
{

}

void CustomListItemWidget::init(const int x, const int y, const int z) {
    QFormLayout* defLy = new QFormLayout;
    this->setLayout(defLy);
    vtb.x = generateNewSpinBox(x, 0.5, -1, 1);
    vtb.y = generateNewSpinBox(y, 0.5, -1, 1);
    vtb.z = generateNewSpinBox(z, 0.5, -1, 1);

    QToolButton *rmButton = new QToolButton;
    rmButton->setText("[X]");

    // Include the widgets into the visual layout
    defLy->addRow(QString::number(0), vtb.x);
    defLy->addRow("", vtb.y);
    defLy->addRow(rmButton, vtb.z);  // Included button in this line.
    QFrame* lineFrame = new QFrame();
    lineFrame->setFrameShape(QFrame::HLine);
    defLy->addRow("", lineFrame);
}
