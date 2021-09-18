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



int ans1 = 0, ans2 = 0;
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
void get_sentiments(QHash<QString, double>& sentiments) {
    QFile file("C:\\Users\\olegv\\UMap\\sentiments.csv");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        exit(1);
    }

    while (!file.atEnd()) {
        QString string_ = file.readLine();
        QString key = string_.split(',').value(0);
        double value = string_.split(',').value(1).toDouble();
        sentiments[key] = value;
    }
    file.close();
}
QGeoPolygon createPolygon(const QJsonArray& points) {
    QGeoPolygon polygon;

    for (QJsonValue point : points) {
        QJsonArray coordinates = point.toArray();
        int x = coordinates.at(0).toDouble();
        int y = coordinates.at(1).toDouble();
        polygon.addCoordinate(QGeoCoordinate(y, x));
    }

    return polygon;
}
void get_states(QList<state>& states) {

    QFile file("C:\\Users\\olegv\\UMap\\states.json");
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
double get_rating(QHash<QString, double>& sentiments, QString text) {
    double result = 0;
    for (int i = 0; i < text.size(); i++) {
        QString new_str = "";
        for (int j = i; j < text.size(); j++) {
            new_str.append(text[j]);
            if (sentiments.contains(new_str)) {
                result += sentiments[new_str];
            }
        }
    }
    return result;
}
void process_tweets(QHash<QString, double>& sentiments, QList<state>& states) {
    QFile file("C:\\Users\\olegv\\UMap\\very_small_tweets.txt");
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
void draw_map() {

}
QList<convert_state> convert(QList<state>& states) {
    QList<convert_state> convert_states;
    for (int i = 0; i < states.size(); i++) {
        QList<QPolygon> polygons;
        for (const QGeoPolygon& geo_polygon: states[i].geo_polygons_) {
            QPolygon polygon;
            for (const QGeoCoordinate& geo_coordinate: geo_polygon.path()) {
                double x_coordinate = geo_coordinate.longitude();
                //x_coordinate *= 20;
                double y_coordinte = geo_coordinate.latitude();
                //y_coordinte  *= 10;
                QPoint point((int)x_coordinate, (int)y_coordinte);
                polygon.append(point);
            }
            polygons.append(polygon);
        }
        double color = states[i].rating_ / 3000;
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
class Widget : public QWidget {
    //Q_OBJECT
    public:
        QList<convert_state> states_;
        ~Widget() {};
        explicit Widget(const QList<convert_state>& states, QWidget *ptr = nullptr) : QWidget(ptr) {
            states_ = states;

            setBackgroundRole(QPalette::Base);
            setAutoFillBackground(true);
        }
    private:
        void paintEvent(QPaintEvent* event) {
            Q_UNUSED(event);
            QPainter painter(this);
            painter.setPen(Qt::black);
            for (const convert_state& state: states_) {
                int b = 255 - state.color_;
                int r= state.color_;
                painter.setBrush(QBrush(QColor(r, 0, b)));

                for (const QPolygon& polygon: state.polygons_) {
                    painter.drawConvexPolygon(polygon);
                }
            }
        }
};

int main(int argc, char *argv[]) {

    QHash<QString, double> sentiments;
    QList<state> states;
    QList<convert_state> convert_states;
    get_sentiments(sentiments);
    get_states(states);
    process_tweets(sentiments, states);
    convert_states = convert(states);

    QApplication aplication(argc, argv);
    Widget window(convert_states);
    window.resize(1900, 1000);
    window.show();
    return QApplication::exec();
}
