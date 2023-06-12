#include "message_widget.hpp"

#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QtWidgets>

MessageWidget::MessageWidget(QWidget *parent) : QWidget(parent) {
  // TODO: your code here...
    QLabel* messageLabel = new QLabel("Message: ");
    m_messageLineLabel = new QLineEdit;

    m_messageLineLabel->setReadOnly(true);

    //QPushButton* button = new QPushButton("Update");
    //button->setToolTip("Click to update");

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(messageLabel);
    layout->addWidget(m_messageLineLabel);
    //layout->addWidget(button);

    //QObject::connect(button, SIGNAL(clicked()), this, SLOT(updateClicked(message)));
    //messageChanged() needs a parameter input
    //below works
    //QObject::connect(this, SIGNAL(messageChanged(QString)), this, SLOT(MessageWidget::info()));

    QObject::connect(this, SIGNAL(messageChanged(QString)), this, SLOT(MessageWidget::info()));
    QObject::connect(this, SIGNAL(messageChanged(QString)), this, SLOT(MessageWidget::error()));

    setLayout(layout);
}

void MessageWidget::info(QString message) {
  // TODO: your code here...
    //m_messageLineLabel->setText(message);
    m_messageLineLabel->setText(message);

    emit messageChanged(message);
}

void MessageWidget::error(QString message) {
  // TODO: your code here...
    
    QPalette pal = m_messageLineLabel->palette();
    //pal.setColor(QPalette::Highlight, QColor("red"));
    //m_messageLineLabel->setPalette(pal);
    m_messageLineLabel->setText(message);

    m_messageLineLabel->selectAll();
    m_messageLineLabel->setStyleSheet("selection-color: white; selection-background-color: red;");

    emit messageChanged(message);
}

void MessageWidget::clear() {
  // TODO: your code here...
    m_messageLineLabel->clear();
}
