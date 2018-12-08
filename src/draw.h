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


public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *e);
    void drawPol(std::vector<QPointFB> &pol, QPainter &painter);
    void mousePressEvent(QMouseEvent *e);
    void setAB(){ab = !ab;}
    void clearAll();


signals:

public slots:
};

#endif // DRAW_H
