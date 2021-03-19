#include <QApplication>

#include "RenderWidget.h"

int main(int argc, char **argv) {
    QApplication application(argc, argv);
    RenderWidget renderer;
    renderer.show();

    return QApplication::exec();
}
