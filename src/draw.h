#ifndef DRAW_H
#define DRAW_H
#include <vector>
#include <QtGui>
#include <QWidget>

#include "qpointfb.h"

class Draw : public QWidget
{
    Q_OBJECT

private:
    std::vector<QPointFB> polA;
    std::vector<QPointFB> polB;
    std::vector<std::vector<QPointFB> > res;
    bool ab;
    std::vector<std::vector<QPointFB> > buff;


public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *e);
    void drawPol(std::vector<QPointFB> &pol, QPainter &painter);
    void mousePressEvent(QMouseEvent *e);
    void setAB(){ab = !ab;}
    void clearAll();
    void clearResults();
    void setRes(std::vector<std::vector<QPointFB> > result){res = result;}
    void setA(std::vector<QPointFB> polA_){polA = polA_;}
    void setB(std::vector<QPointFB> polB_){polB = polB_;}
    std::vector<QPointFB> getA(){return polA;}
    std::vector<QPointFB> getB(){return polB;}
    void setBuff(std::vector<std::vector<QPointFB> > buff_) {buff=buff_;}






signals:

public slots:
};

#endif // DRAW_H
