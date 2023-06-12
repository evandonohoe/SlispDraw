#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <string>

#include <QWidget>
#include <QLabel>

#include "message_widget.hpp"
#include "canvas_widget.hpp"
#include "repl_widget.hpp"
#include "interpreter_semantic_error.hpp"

#include "qt_interpreter.hpp"

class MainWindow: public QWidget{
  Q_OBJECT

public:

  MainWindow(QWidget * parent = nullptr);
  MainWindow(std::string filename, QWidget * parent = nullptr);

  int executeFromFile(std::string fileName);

private:
    QtInterpreter* m_interp;
    MessageWidget* m_messageWidget;
    CanvasWidget* m_canvasWidget;
    REPLWidget* m_replEntry;
};


#endif
