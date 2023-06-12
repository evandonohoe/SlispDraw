#include <string>
#include <iostream>

#include <QApplication>
#include <QDebug>
#include <QtWidgets>

#include "main_window.hpp"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  std::string filename;

  if(argc == 2){
    filename = argv[1];
  }
  if(argc > 2){
    std::cerr << "Error: invalid number of arguments to sldraw" << std::endl;
    return EXIT_FAILURE;
  }

  MainWindow w(filename);
  w.setMinimumSize(800,400);
  w.show();

  /*
  QGraphicsScene scene;
  QGraphicsRectItem* rect = new QGraphicsRectItem(QRectF(0, 0, 100, 100));
  rect->setBrush(QBrush(Qt::red));
  scene.addItem(rect);

  QGraphicsView view(&scene);
  view.show();*/

  return app.exec();
}
