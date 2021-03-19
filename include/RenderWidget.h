#ifndef RENDER_WIDGET_H
#define RENDER_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>

#include "Scene.h"

class RenderWidget : public QWidget {
    Q_OBJECT
private:
    QLabel imageLabel;
    Scene scene;

public:
    RenderWidget();
    ~RenderWidget() override;
};

#endif