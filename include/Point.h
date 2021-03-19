#ifndef POINT_H
#define POINT_H

#include <QVector3D>

class Point {
private:
    QVector3D position, normal;

public:
    Point(const QVector3D &position, const QVector3D &normal);
    ~Point();
    QVector3D getPosition() const;
    QVector3D getNormal() const;
};

#endif