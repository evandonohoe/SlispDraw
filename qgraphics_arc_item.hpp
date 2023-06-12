#ifndef QGRAPHIC_ARC_ITEM_HPP
#define QGRAPHIC_ARC_ITEM_HPP

#include <QGraphicsEllipseItem>

class QGraphicsArcItem: public QGraphicsEllipseItem{

public:

  QGraphicsArcItem(qreal x, qreal y, qreal width, qreal height, double span, double start,
		   QGraphicsItem *parent = nullptr);

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
	//QRect bounding_rect;
	int span_angle, start_angle;

};


#endif
