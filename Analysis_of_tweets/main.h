#ifndef MAIN_H
#define MAIN_H

#include <QGeoPolygon>

class state {
    public:
        QList<QGeoPolygon> geo_polygons_;
        QString name_;
        double rating_;
        state(QList<QGeoPolygon> geo_polygons, QString name, double rating) {
            rating_ = rating;
            name_ = name;
            geo_polygons_ = geo_polygons;
        }
};
class convert_state {
    public:
        QList<QPolygon> polygons_;
        double color_;
        convert_state(QList<QPolygon> polygons, double color) {
            color_ = color;
            polygons_ = polygons;
        }
};

#endif // MAIN_H
