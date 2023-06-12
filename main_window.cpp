#include "main_window.hpp"

#include <iostream>


#include <QLayout>
#include <QtWidgets>
#include <QDebug>

#include <QLayout>
#include <QLineEdit>

#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

int MainWindow::executeFromFile(std::string fileName) {
	ifstream inSlpFile;
	inSlpFile.open(fileName);
	string line;
	string sd_string;

	int EXIT = 0;
	if (inSlpFile.is_open())
	{
		if (inSlpFile.peek() == EOF) {
			inSlpFile.close();
		}
		else {
			string conc;
			while (getline(inSlpFile, line))
			{
				size_t comment = line.find(";");
				line = line.substr(0, comment);
				conc += line;
			}

			QString entry = QString::fromStdString(conc);

			m_interp->parseAndEvaluate(entry);

		}
		inSlpFile.close();
	}
	else {
		std::cerr << "Error: Failed to open file of program: " << endl;
		EXIT = EXIT_FAILURE;
	}
	return EXIT;
}

MainWindow::MainWindow(QWidget * parent): MainWindow("", parent){

}

MainWindow::MainWindow(std::string filename, QWidget * parent): QWidget(parent){

	m_interp = new QtInterpreter();

    m_messageWidget = new MessageWidget();

    m_canvasWidget = new CanvasWidget();

    m_replEntry = new REPLWidget();

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(m_messageWidget);
    layout->addWidget(m_canvasWidget);
    layout->addWidget(m_replEntry);
    setLayout(layout);
    setWindowTitle("sldraw");

    //connect REPL output to QtInterpreter input
    connect(m_replEntry, &REPLWidget::lineEntered, m_interp, &QtInterpreter::parseAndEvaluate);
   
    //connect interpreter output to message input
    connect(m_interp, &QtInterpreter::info, m_messageWidget, &MessageWidget::info);

    // connect interpreter output to message input
    connect(m_interp, &QtInterpreter::error, m_messageWidget, &MessageWidget::error);

    //connect interpreter output to canvas input
    connect(m_interp, &QtInterpreter::drawGraphic, m_canvasWidget, &CanvasWidget::addGraphic);

	if (!(filename.empty())) {
		executeFromFile(filename);
	}

}








