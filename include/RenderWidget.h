#ifndef RENDER_WIDGET_H
#define RENDER_WIDGET_H

#include <QVector3D>
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>

#include "Scene.h"

class RenderWidget : public QWidget {
    Q_OBJECT
private:
    QHBoxLayout horizontal;
    QLabel imageLabel;
    Scene scene;

public:
    RenderWidget();
    ~RenderWidget() override;
};

#endif