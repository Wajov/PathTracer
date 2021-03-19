#include "RenderWidget.h"

RenderWidget::RenderWidget() : QWidget() {
    scene = Scene("scene/cornellbox/cornellbox.obj");
    imageLabel.setParent(this);
    QVector3D position(0.0f, 0.0f, 2.5f);
    QVector3D center(0.0f, 0.0f, 0.0f);
    QVector3D up(0.0f, 1.0f, 0.0f);
    imageLabel.setPixmap(QPixmap::fromImage(scene.render(position, center, up, 60.0f)));
}

RenderWidget::~RenderWidget() {}