#ifndef REPL_WIDGET_HPP
#define REPL_WIDGET_HPP

#include <QWidget>
#include <QLineEdit>
#include <QString>
#include <QVector>

class REPLWidget: public QWidget{
Q_OBJECT

public:

  REPLWidget(QWidget * parent = nullptr);

  bool eventFilter(QObject* obj, QEvent* event);

signals:

  void lineEntered(QString entry);


private slots:


  void changed();

private:

    QLineEdit* m_REPLLineLabel;

    QStringList m_commandHistory;

    int m_commandHistoryIndex;


    //vector that stores history strings
    //index that keeps track of how many ups or downs pressed

};

#endif
