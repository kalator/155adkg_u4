#include "draw.h"

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    ab = true;
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

    painter.begin(this);

    //Draw polygon A
    painter.setPen(Qt::green);
    drawPol(polA, painter);

    //Draw polygon B
    painter.setPen(Qt::blue);
    drawPol(polB, painter);

    //Draw result
    painter.setPen(Qt::red);
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
