#pragma once

#include <QWidget>
#include <QPen>
#include <QSet>
#include "main.h"

class widget : public QWidget {

    Q_OBJECT

    QList<convert_state> states_;

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    explicit widget(const QList<convert_state>& states, QWidget *parent = nullptr);
};
