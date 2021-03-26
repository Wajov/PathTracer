#include "RenderWidget.h"

RenderWidget::RenderWidget() : QWidget() {
    sceneLabel.setParent(this);
    sceneLabel.setText("Select Scene:");
    sceneButton0.setParent(this);
    sceneButton0.setText("Cornell Box");
    sceneButton0.setChecked(true);
    sceneButton1.setParent(this);
    sceneButton1.setText("Car (Config 1)");
    sceneButton2.setParent(this);
    sceneButton2.setText("Car (Config 2)");
    sceneButton3.setParent(this);
    sceneButton3.setText("Dining Room");

    parameterLabel.setParent(this);
    parameterLabel.setText("Parameters:");
    validator.setParent(this);
    sppLabel.setParent(this);
    sppLabel.setText("Sample per Pixel:");
    sppEdit.setParent(this);
    sppEdit.setValidator(&validator);
    sppEdit.setText(QString::number(SAMPLE_PER_PIXEL));
    splLabel.setParent(this);
    splLabel.setText("Sample per Light:");
    splEdit.setParent(this);
    splEdit.setValidator(&validator);
    splEdit.setText(QString::number(SAMPLE_PER_LIGHT));
    iprLabel.setParent(this);
    iprLabel.setText("Iteration per Refresh:");
    iprEdit.setParent(this);
    iprEdit.setValidator(&validator);
    iprEdit.setText(QString::number(ITERATION_PER_REFRESH));
    grid.addWidget(&sppLabel, 0, 0);
    grid.addWidget(&sppEdit, 0, 1);
    grid.addWidget(&splLabel, 1, 0);
    grid.addWidget(&splEdit, 1, 1);
    grid.addWidget(&iprLabel, 3, 0);
    grid.addWidget(&iprEdit, 3, 1);

    iterationLabel.setParent(this);
    iterationLabel.setText(QString("Iteration: 0/%1").arg(SAMPLE_PER_PIXEL));
    calculateButton.setParent(this);
    calculateButton.setText("Calculate");
    connect(&calculateButton, SIGNAL(pressed()), this, SLOT(calculate()));
    saveButton.setParent(this);
    saveButton.setText("Save");
    connect(&saveButton, SIGNAL(pressed()), this, SLOT(save()));

    vertical.addWidget(&sceneLabel);
    vertical.addWidget(&sceneButton0);
    vertical.addWidget(&sceneButton1);
    vertical.addWidget(&sceneButton2);
    vertical.addWidget(&sceneButton3);
    vertical.addWidget(&parameterLabel);
    vertical.addLayout(&grid);
    vertical.addWidget(&iterationLabel);
    vertical.addWidget(&calculateButton);
    vertical.addWidget(&saveButton);
    vertical.addStretch();

    imageLabel.setParent(this);
    horizontal.addLayout(&vertical);
    horizontal.addWidget(&imageLabel);

    setLayout(&horizontal);
}

RenderWidget::~RenderWidget() {}

void RenderWidget::refresh(const std::vector<std::vector<QVector3D>> &sum, const int sample) {
    for (int i = 0; i < sum.size(); i++)
        for (int j = 0; j < sum[i].size(); j++)
            image.setPixel(i, j, vectorToColor(sum[i][j] / (float)sample).rgb());

    imageLabel.setPixmap(QPixmap::fromImage(image));
}

void RenderWidget::calculate() {
    SAMPLE_PER_PIXEL = sppEdit.text().toInt();
    SAMPLE_PER_LIGHT = splEdit.text().toInt();
    ITERATION_PER_REFRESH = iprEdit.text().toInt();

    QVector3D position, center, up;
    float fovy;
    int width, height;
    if (sceneButton0.isChecked()) {
        scene = Scene("scene/cornellbox/cornellbox.obj");
        position = QVector3D(0.0f, 0.0f, 2.5f);
        center = QVector3D(0.0f, 0.0f, 0.0f);
        up = QVector3D(0.0f, 1.0f, 0.0f);
        fovy = 60.0f;
        width = 720;
        height = 720;
    } else if (sceneButton1.isChecked()) {
        scene = Scene("scene/car/car.obj", "scene/car/environment_day.hdr");
        position = QVector3D(8.22f, -0.61f, -9.80f);
        center = QVector3D(7.514f, -0.702f, -9.097f);
        up = QVector3D(-0.065f, 0.996f, 0.065f);
        fovy = 45.0f;
        width = 1280;
        height = 720;
    } else if (sceneButton2.isChecked()) {
        scene = Scene("scene/car/car.obj", "scene/car/environment_dusk.hdr");
        position = QVector3D(5.72f, 0.12f, 9.55f);
        center = QVector3D(5.085f, -0.131f, 8.819f);
        up = QVector3D(-0.165f, 0.968f, -0.189f);
        fovy = 45.0f;
        width = 1280;
        height = 720;
    } else if (sceneButton3.isChecked()) {
        scene = Scene("scene/diningroom/diningroom.obj", "scene/diningroom/environment.hdr");
        position = QVector3D(0.0f, 12.72f, 31.85f);
        center = QVector3D(0.0f, 12.546f, 30.865f);
        up = QVector3D(0.0f, 0.985f, -0.174f);
        fovy = 45.0f;
        width = 1280;
        height = 720;
    }

    QVector3D f = (center - position).normalized();
    QVector3D u = up.normalized();
    QVector3D l = QVector3D::crossProduct(u, f);
    float scale = 2.0f * std::tan(fovy / 360.0f * PI) / (float)height;
    QVector3D du = u * scale;
    QVector3D dl = l * scale;
    QVector3D o = position + f + (du * (float)height + dl * (float)width) * 0.5f;

    std::vector<std::vector<QVector3D>> sum(width, std::vector<QVector3D>(height, QVector3D(0.0f, 0.0f, 0.0f)));
    image = QImage(width, height, QImage::Format_RGB32);
    for (int i = 0; i < SAMPLE_PER_PIXEL; i++) {
        scene.sample(position, o, du, dl, sum);
        iterationLabel.setText(QString("Iteration: %1/%2").arg(i + 1).arg(SAMPLE_PER_PIXEL));
        QCoreApplication::processEvents();
        if ((i + 1) % ITERATION_PER_REFRESH == 0)
            refresh(sum, i + 1);
    }
    if (SAMPLE_PER_PIXEL % ITERATION_PER_REFRESH > 0)
        refresh(sum, SAMPLE_PER_PIXEL);
}

void RenderWidget::save() {
    QString path = QFileDialog::getSaveFileName(this, "Save Image", ".", "");
    image.save(path);
}