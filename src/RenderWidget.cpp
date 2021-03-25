#include "RenderWidget.h"

RenderWidget::RenderWidget() : QWidget() {
    imageLabel.setParent(this);

//    scene = Scene("scene/cornellbox/cornellbox.obj");
//    QVector3D position(0.0f, 0.0f, 2.5f);
//    QVector3D center(0.0f, 0.0f, 0.0f);
//    QVector3D up(0.0f, 1.0f, 0.0f);
//    float fovy = 60.0f;
//    int width = 720, height = 720;

//    scene = Scene("scene/extra/scene02.obj");
//    QVector3D position(2, 11.464, 23.8682);
//    QVector3D center(2, 9.464, 18.8682);
//    QVector3D up(0, 1, 0);
//    float fovy = 50;
//    int width = 600, height = 400;

    scene = Scene("scene/car/car.obj", "scene/car/environment_day.hdr");
    QVector3D position(8.22f, -0.61f, -9.80f);
    QVector3D center(7.514f, -0.702f, -9.097f);
    QVector3D up(-0.065f, 0.996f, 0.065f);
    float fovy = 45.0f;
    int width = 1280, height = 720;
    imageLabel.setPixmap(QPixmap::fromImage(scene.render(position, center, up, fovy, width, height)));

    horizontal.addWidget(&imageLabel);

    setLayout(&horizontal);
}

RenderWidget::~RenderWidget() {}