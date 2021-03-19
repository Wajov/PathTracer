#ifndef RAY_H
#define RAY_H

#include <QVector3D>

class Ray {
private:
    QVector3D origin, direction;

public:
    Ray(const QVector3D &origin, const QVector3D &direction);
    ~Ray();
    QVector3D getOrigin() const;
    QVector3D getDirection() const;
};

#endif