#ifndef MESSAGE_WINDOW_HPP
#define MESSAGE_WINDOW_HPP

#include <QString>
#include <QWidget>

class QLineEdit;

class MessageWidget : public QWidget {
  Q_OBJECT

public:
  MessageWidget(QWidget *parent = nullptr);

public slots:
    //void updateClicked(QString message);

    void info(QString message);

    void error(QString message);

    void clear();

signals:
    void messageChanged(QString message);

private:
    QLineEdit* m_messageLineLabel;
};

#endif
