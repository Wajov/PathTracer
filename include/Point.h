#ifndef POINT_H
#define POINT_H

#include <QVector3D>
#include <QVector2D>

class Point {
private:
    QVector3D position, normal;
    QVector2D uv;

public:
    Point();
    Point(const QVector3D &position, const QVector3D &normal, const QVector2D &uv);
    ~Point();
    QVector3D getPosition() const;
    QVector3D getNormal() const;
    QVector2D getUV() const;
};

#endif