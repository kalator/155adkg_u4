#include "draw.h"

#include <QDebug>
#include <fstream>

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    ab = true;
    draw_pol = false;
    /*
    QPointFB p1(0,0);
    QPointFB p2(100,0);
    QPointFB p3(0,100);

    polA.push_back(p1);
    polA.push_back(p2);
    polA.push_back(p3);

    QPointFB p4(50,10);
    QPointFB p5(150,10);
    QPointFB p6(50,110);

    polB.push_back(p4);
    polB.push_back(p5);
    polB.push_back(p6);
    */
}

void Draw::paintEvent(QPaintEvent *e)
{
    QPainter painter (this);

    //Draw polygon A
    painter.setPen(Qt::green);
    drawPol(polA, painter);

    //Draw polygon B
    painter.setPen(Qt::blue);
    drawPol(polB, painter);

    //Draw result
    QPen pen_result(Qt::red, 3);
    painter.setPen(pen_result);
    for(std::vector<QPointFB> vec: res)
    {
        drawPol(vec, painter);
    }

    //Draw offset
    painter.setPen(Qt::cyan);
    for(std::vector<QPointFB> b: buff)
    {
        drawPol(b, painter);
    }
}

void Draw::drawPol(std::vector<QPointFB> &pol, QPainter &painter)
{
    QPolygon pol_q;

    for(QPointFB p: pol)
    {
        QPoint pb (p.x(), p.y());
        pol_q.append(pb);
    }

    painter.drawPolygon(pol_q);
}

void Draw::mousePressEvent(QMouseEvent *e)
{
    if(draw_pol == false)
        return;

    QPointFB p(e->x(), e->y());

    //Add to polA
    if(ab)
    {
        polA.push_back(p);
    }

    //Add to polB
    else
    {
        polB.push_back(p);
    }

    repaint();
}

void Draw::clearAll()
{
    // Clear all in canvas
    polA.clear();
    polB.clear();
    res.clear();
}

void Draw::clearResults()
{
    // Clear results in canvas
    res.clear();
}

void Draw::loadPoints(std::string path, QSizeF &canvas_size)
{
    std::ifstream points_file;
    points_file.open(path);

    //check if points_file is correctly open (or if it exists)
    if(!points_file.is_open())
    {
        return;
    }


    polA.clear();
    polB.clear();

    //go through file and load points into poly_pol (storing all polygons)
    double min_x = std::numeric_limits<double>::max();
    double min_y = std::numeric_limits<double>::max();
    double max_x = std::numeric_limits<double>::min();
    double max_y = std::numeric_limits<double>::min();

    while(points_file.good())
    {
        int no_of_points;
        double x,y;

        points_file >> no_of_points;
        for(int i = 0; i < no_of_points; i++)
        {
            points_file >> x;
            points_file >> y;
            polA.push_back(QPointFB(x, y));
            if(x < min_x) min_x = x;
            if(x > max_x) max_x = x;
            if(y < min_y) min_y = y;
            if(y > max_y) max_y = y;
        }

        points_file >> no_of_points;
        for(int i = 0; i < no_of_points; i++)
        {
            points_file >> x;
            points_file >> y;
            polB.push_back(QPointFB(x, y));
            if(x < min_x) min_x = x;
            if(x > max_x) max_x = x;
            if(y < min_y) min_y = y;
            if(y > max_y) max_y = y;
        }

        break;

    }

    //scale points to canvas size
    double h = canvas_size.height() - 40;
    double w = canvas_size.width() - 40;

    double x_coef = w/(max_x-min_x);
    double y_coef = h/(max_y-min_y);

    for(unsigned int i = 0; i < polA.size(); i++)
    {
        polA[i].setX((polA[i].x()-min_x)*x_coef);
        polA[i].setY((polA[i].y()-min_y)*y_coef);
    }

    for(unsigned int i = 0; i < polB.size(); i++)
    {
        polB[i].setX((polB[i].x()-min_x)*x_coef);
        polB[i].setY((polB[i].y()-min_y)*y_coef);
    }
    points_file.close();
}
