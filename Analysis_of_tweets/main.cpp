#include "widget.h"
#include "main.h"
#include <QApplication>
#include <QPushButton>
#include <QString>
#include <QFile>
#include <QList>
#include <QHash>
#include <QtDebug>
#include <QRegularExpression>
#include <QGeoPolygon>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QWidget>
#include <QPainter>



int ans1 = 0, ans2 = 0, string_max_size = 0;
const int md = 1e9 + 7;
void get_sentiments(QHash<int, double>& sentiments) {
    QFile file("C:\\Users\\olegv\\Analysis_of_tweets\\sentiments.csv");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        exit(1);
    }

    while (!file.atEnd()) {
        QString string_ = file.readLine();
        QString key = string_.split(',').value(0);
        string_max_size = std::max(string_max_size, key.size());
        long long key_ = 0;
        for (int i = 0; i < key.size(); i++) {
            key_ = (key_ * 355 + key[i].unicode()) % md;
            if (!sentiments.contains(key_)) {
                sentiments[key_] = 0;
            }
        }
        double value = string_.split(',').value(1).toDouble();
        sentiments[key_] = value;
    }
    file.close();
}
QGeoPolygon createPolygon(const QJsonArray& points) {
    QGeoPolygon polygon;

    for (QJsonValue point : points) {
        QJsonArray coordinates = point.toArray();
        double x = coordinates.at(0).toDouble();
        double y = coordinates.at(1).toDouble();
        polygon.addCoordinate(QGeoCoordinate(y, x));
    }

    return polygon;
}
void get_states(QList<state>& states) {

    QFile file("C:\\Users\\olegv\\Analysis_of_tweets\\states.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        exit(1);
    }
    QString string_ = file.readLine();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(string_.toUtf8());

    QJsonObject object = document.object();
    QStringList names = object.keys();

    for (const QString& name : names) {

        QJsonArray polygons = object.value(name).toArray();
        QList<QGeoPolygon> statePolygons;

        if (polygons.size() == 1) {
            QGeoPolygon statePolygon = createPolygon(polygons.first().toArray());
            statePolygons.append(statePolygon);

        } else {
            for (QJsonValue polygon : polygons) {
                QJsonArray points = polygon.toArray().first().toArray();
                QGeoPolygon statePolygon = createPolygon(points);
                statePolygons.append(statePolygon);
            }
        }
        states.append(state(statePolygons, name, 0));
    }
    file.close();
}
void update_state(QList<state>& states,double x_coordinate, double y_coordinate, double rating) {
    QGeoCoordinate point(x_coordinate, y_coordinate);
    int flag = 0;
    for (int i = 0; i < states.size(); i++) {
        for (const QGeoPolygon& polygon : states[i].geo_polygons_) {
            if (polygon.contains(point)) {
                states[i].rating_ += rating;
                flag = 1;
                break;
            }
        }
        if (flag == 1) {
            break;
        }
    }
    if (flag == 0) {
        ans1++;
    } else {ans2++;}
}
double get_rating(QHash<int, double>& sentiments, QString text) {
    double result = 0;
    for (int i = 0; i < text.size(); i++) {
        long long key = 0;
        for (int j = i; j < text.size(); j++) {
            key = (key * 355 + text[j].unicode()) % md;
            if (sentiments.contains(key)) {
                result += sentiments[key];
            }
            else {
                break;
            }
        }
    }
    return result;
}
void process_tweets(QHash<int, double>& sentiments, QList<state>& states) {
    QFile file("C:\\Users\\olegv\\Analysis_of_tweets\\very_small_tweets.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        exit(1);
    }
    QRegularExpression for_coordinates("^\\[(\\-?\\d+\\.\\d+), (\\-?\\d+\\.\\d+)\\]");
    QRegularExpression for_text("(?<=\\d{4}\\-\\d{2}-\\d{2}\\s\\d{2}:\\d{2}:\\d{2}\\s).*$");

    while (!file.atEnd()) {
        QString string_ = file.readLine();

        QRegularExpressionMatch match_for_coordinates = for_coordinates.match(string_);
        double x_coordinate = match_for_coordinates.captured(1).toDouble();
        double y_coordinate = match_for_coordinates.captured(2).toDouble();
        QRegularExpressionMatch match_for_text = for_text.match(string_);
        QString text = match_for_text.captured();
        double rating = get_rating(sentiments, text);
        update_state(states, x_coordinate, y_coordinate, rating);
    }
    file.close();
}

double map(double value, double min_in, double max_in, double min_out, double max_out) {
    return (value - min_in) * (max_out - min_out) / (max_in - min_in) + min_out;
}
double max_rating = -2e9, min_rating = 2e9;
QList<convert_state> convert(QList<state>& states) {
    QList<convert_state> convert_states;
    for (int i = 0; i < states.size(); i++) {
        max_rating = std::max(states[i].rating_, max_rating);
        min_rating = std::min(states[i].rating_, min_rating);
    }
    for (int i = 0; i < states.size(); i++) {
        QList<QPolygon> polygons;
        for (const QGeoPolygon& geo_polygon: states[i].geo_polygons_) {
            QPolygon polygon;
            for (const QGeoCoordinate& geo_coordinate: geo_polygon.path()) {
                double x_coordinate = geo_coordinate.longitude();
                if (x_coordinate > 0) {
                    x_coordinate -= 180.0;
                }
                x_coordinate = 200.0 + x_coordinate;
                x_coordinate *= 10.0;
                double y_coordinate = geo_coordinate.latitude();
                y_coordinate = 100.0 - y_coordinate;

                y_coordinate  *= 10.0;

                QPoint point((int)x_coordinate, (int)y_coordinate);
                polygon.append(point);
            }
            polygons.append(polygon);
        }
        double color = map(states[i].rating_, min_rating, (max_rating + min_rating) / 3, 0, 255);
        if (color < 0) {
            color = 0;
        }
        if (color > 255) {
            color = 255;
        }
        convert_states.append(convert_state(polygons, color));
    }
    return convert_states;
}

int main(int argc, char *argv[]) {

    QHash<int, double> sentiments;
    QList<state> states;
    QList<convert_state> convert_states;
    get_sentiments(sentiments);
    get_states(states);
    process_tweets(sentiments, states);
    convert_states = convert(states);
    QApplication aplication(argc, argv);
    widget window(convert_states);
    window.resize(1900, 1000);
    window.show();

    return QApplication::exec();
}
