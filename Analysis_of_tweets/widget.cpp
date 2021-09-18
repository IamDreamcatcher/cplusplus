#include <QPainter>
#include "widget.h"
#include "main.h"

#include <QDebug>

void widget::paintEvent(QPaintEvent *event) {

    QPainter painter(this);
    painter.setPen(Qt::black);

    for (const convert_state& state : states_) {
        int b = 255 - state.color_;
        int r = state.color_;
        painter.setBrush(QBrush(QColor(r, 0, b)));

        for (const QPolygon& polygon : state.polygons_) {
            painter.drawConvexPolygon(polygon);
        }
    }
}

widget::widget(const QList<convert_state> &states, QWidget *parent) {
    states_ = states;

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}
