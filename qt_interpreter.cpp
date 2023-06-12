#include "qt_interpreter.hpp"
#define _USE_MATH_DEFINES
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <QtMath>
#include <fstream>

#include <QBrush>
#include <QDebug>

#include "qgraphics_arc_item.hpp"



#include "interpreter_semantic_error.hpp"

QtInterpreter::QtInterpreter(QObject * parent): QObject(parent){
	Interpreter interp;
}

double distance(Number x1, Number y1, Number x2, Number y2) {
    double dy = y2 - y1;
    double dx = x2 - x1;
    return sqrt(dx * dx + dy * dy);
}

QGraphicsArcItem* makeArc(Number c_x, Number c_y, Number s_x, Number s_y, Number span) {

    double radius = distance(c_x, c_y, s_x, s_y);

    double rect_x = 2 * radius;
    double rect_y = 2 * radius;

    double x = c_x - radius;
    double y = c_y - radius;

    double dx = s_x - c_x;
    double dy = s_y - c_y;

    double start_angle = atan2(-dy, dx);

    double span_angle = qRadiansToDegrees(span);

    double start_angle_deg = qRadiansToDegrees(start_angle);

    QGraphicsItem* parent = nullptr;

    QGraphicsArcItem* arc_item = new QGraphicsArcItem(x, y, rect_x, rect_y, span_angle, start_angle_deg, parent);

    return arc_item;

}


void QtInterpreter::parseAndEvaluate(QString entry){
    Interpreter interp;

    std::string std_entry = entry.toStdString();
    std::istringstream iss(std_entry);
    bool ok = interp.parse(iss);
    if (!ok) {
        std::cerr << "Error: Failed to parse: " << std_entry << std::endl;
        std::string msg = "Error: Failed to parse: " + std_entry;
        QString err_msg = QString::fromStdString(msg);
        emit error(err_msg);
        return;
    }

    try {
        Expression result = interp.eval();

        std::string str_exp = expToString(result);

        QString qResult = QString::fromStdString(str_exp);
        if (!(str_exp == "(NoneTypeVal)")) {
            emit info(qResult);
        }
    }
    catch (const InterpreterSemanticError& ex) {
        qDebug() << "In error";
        std::cerr << "Error: Failed to eval program: " << ex.what() << std::endl;
        std::string msg = "Error: Failed to eval program: " + std::to_string(*ex.what());
        QString err_msg = QString::fromStdString(msg);
        emit error(err_msg);
        return;
    }

    std::vector<Expression> draw_list = interp.getDrawList();
    qDebug() << "Before if";
    if (!draw_list.empty()) {
        qDebug() << "In if";
        for (auto& exp : draw_list) {
            switch (exp.head.type)
            {
            case PointType:
            {
                qDebug() << "In point";
                Number x_coord = std::get<0>(exp.head.value.start_coords);
                Number y_coord = std::get<1>(exp.head.value.start_coords);
                QGraphicsEllipseItem* circle = new QGraphicsEllipseItem(x_coord - 2, y_coord - 2, 4, 4);
                emit drawGraphic(circle);
                break;
            }
            case LineType:
            {
                qDebug() << "In Line";
                Number x1_coord = std::get<0>(exp.head.value.start_coords);
                Number y1_coord = std::get<1>(exp.head.value.start_coords);
                Number x2_coord = std::get<0>(exp.head.value.end_coords);
                Number y2_coord = std::get<1>(exp.head.value.end_coords);
                QGraphicsLineItem* line = new QGraphicsLineItem(x1_coord, y1_coord, x2_coord, y2_coord);
                emit drawGraphic(line);
                break;
            }
            case ArcType:
            {
                qDebug() << "In Arc";
                Number center_x = std::get<0>(exp.head.value.start_coords);
                Number center_y = std::get<1>(exp.head.value.start_coords);
                Number start_x = std::get<0>(exp.head.value.end_coords);
                Number start_y = std::get<1>(exp.head.value.end_coords);

                Number radian_span = exp.head.value.num_value;
                qDebug() << "radian_span:" << radian_span;

                QGraphicsArcItem* arc = makeArc(center_x, center_y, start_x, start_y, radian_span);

                emit drawGraphic(arc);
                break;
            }
            }
        }
    }

}