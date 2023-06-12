#include "qgraphics_arc_item.hpp"

#include <cmath>

#include <QDebug>
#include <QPainter>



QGraphicsArcItem::QGraphicsArcItem(qreal x, qreal y, qreal width, qreal height, double span, double start, QGraphicsItem *parent) {
    this->setRect(x, y, width, height);
    start_angle = start;
    span_angle = span;
}

void QGraphicsArcItem::paint(QPainter *painter,
                             const QStyleOptionGraphicsItem *option,
                             QWidget *widget) {
 
    painter->drawArc(this->boundingRect(), start_angle * 16, span_angle * 16);
}
