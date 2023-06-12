#include "repl_widget.hpp"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QDebug>
#include <QKeyEvent>

REPLWidget::REPLWidget(QWidget * parent): QWidget(parent){
  // TODO: your code here
    m_commandHistoryIndex = 0;
    QLabel* REPLLabel = new QLabel("slisp> ");
    m_REPLLineLabel = new QLineEdit;

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(REPLLabel);
    layout->addWidget(m_REPLLineLabel);

    m_REPLLineLabel->installEventFilter(this);
    m_REPLLineLabel->setFocus();

    connect(m_REPLLineLabel, SIGNAL(returnPressed()), this, SLOT(changed()));

    //connect(m_REPLLineLabel, SIGNAL(returnPressed()), this, SLOT(changed()));
    

    setLayout(layout);
}

void REPLWidget::changed() {
    QString input = m_REPLLineLabel->text();

    //m_commandHistory.push_back(input);
    //m_commandHistoryIndex++;
    m_commandHistoryIndex = m_commandHistory.count();
    m_commandHistory.append(input);

    //qDebug() << "Input: " << input;
    m_REPLLineLabel->clear();
    emit lineEntered(input);
}

bool REPLWidget::eventFilter(QObject* obj, QEvent* event)
{
    bool retVal = false;
    if (obj == m_REPLLineLabel && event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Up) {
            //emit upPressed();
            if (m_commandHistoryIndex > 0) {
                m_commandHistoryIndex--;
                m_REPLLineLabel->setText(m_commandHistory[m_commandHistoryIndex]);
            }
            retVal = true;
        }
        else if (keyEvent->key() == Qt::Key_Down) {
            //emit downPressed();
            if (m_commandHistoryIndex < m_commandHistory.count() - 1) {
                m_commandHistoryIndex++;
                m_REPLLineLabel->setText(m_commandHistory[m_commandHistoryIndex]);
            }
            else {
                m_commandHistoryIndex = m_commandHistory.count();
                m_REPLLineLabel->clear();
            }
            retVal = true;
        }
        else {
            retVal = false;
        }
    }
    else {
        return QObject::eventFilter(obj, event);
    }
    return retVal;
}
