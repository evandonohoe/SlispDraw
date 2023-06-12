#include "canvas_widget.hpp"

#include <QWidget>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLayout>

CanvasWidget::CanvasWidget(QWidget * parent): QWidget(parent){
  // TODO: your code here...
	scene = new QGraphicsScene(this);
	//QGraphicsRectItem* rect = new QGraphicsRectItem(100, 100, 100, 100);
	//rect->setBrush(QBrush(Qt::red));
	//scene->addItem(rect);
	
	//rect->setZValue(1);
	//scene->setSceneRect(QRectF(0, 0, 700, 150));



	QObject::connect(this, SIGNAL(canvasChanged(QGraphicsItem)), this, SLOT(addGraphic(QGraphicsItem)));

	QGraphicsView* view = new QGraphicsView(scene);

	view->setMaximumSize(800, 300);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(view);
	setLayout(layout);

}

void CanvasWidget::addGraphic(QGraphicsItem * item){
  // TODO: your code here...
	//qDebug() << "In3";
	//qDebug() << "Adding graphic item: " << item->type();
	scene->addItem(item);
	//scene->update();
	emit canvasChanged(item);
}
