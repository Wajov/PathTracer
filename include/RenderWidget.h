#ifndef RENDER_WIDGET_H
#define RENDER_WIDGET_H

#include <vector>

#include <QVector3D>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QRadioButton>
#include <QLineEdit>
#include <QIntValidator>
#include <QPushButton>
#include <QString>
#include <QImage>
#include <QCoreApplication>
#include <QPixmap>
#include <QFileDialog>

#include "ConfigHelper.h"
#include "Scene.h"

class RenderWidget : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout vertical;
    QHBoxLayout horizontal;
    QGridLayout grid;
    QLabel sceneLabel, parameterLabel, sppLabel, splLabel, iprLabel, iterationLabel, imageLabel;
    QRadioButton sceneButton0, sceneButton1, sceneButton2, sceneButton3, sceneButton4;
    QLineEdit sppEdit, splEdit, iprEdit;
    QIntValidator validator;
    QPushButton calculateButton, saveButton;
    Scene scene;
    QImage image;
    void refresh(const std::vector<std::vector<QVector3D>> &sum, const int sample);

public:
    RenderWidget();
    ~RenderWidget() override;

private slots:
    void calculate();
    void save();
};

#endif