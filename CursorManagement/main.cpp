#include <iostream>

#include <QApplication>
#include <QTimer>

void showCursorPosition() {
    // Detect the screen where the cursor is
    QPoint globalCursorPos = QCursor::pos();

    std::cout << "Cursor pos: (" << globalCursorPos.rx() << ", "
                                 << globalCursorPos.ry() << ")" << std::endl;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QTimer *timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, showCursorPosition);
    timer->start(33);

    return a.exec();
}
