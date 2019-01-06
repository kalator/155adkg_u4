#ifndef QPOINTFB_H
#define QPOINTFB_H
#include <QtGui>

#include "types.h"

class QPointFB : public QPointF
{
        private:
        double alfa;
        double beta;
        bool inters;
        TPointPolygon pos;

    public:
        QPointFB():QPointF(), alfa(0.0), beta(0.0), inters(false), pos(ON){};
        QPointFB(double x_, double y_, double alfa_, double beta_, bool inters_,
                 TPointPolygon pos_) :QPointF(x_, y_), alfa(alfa_), beta(beta_),
              inters(inters_), pos(pos_){};
        QPointFB(double x_, double y_) : QPointF(x_, y_), alfa(0.0), beta(0.0), inters(false), pos(ON){}
        double getAlfa(){return alfa;}
        void setAlfa(double alfa_){alfa = alfa_;}
        double getBeta(){return beta;}
        void setBeta(double beta_){beta = beta_;}
        bool getInters(){return inters;}
        void setInters(bool inters_){inters = inters_;}
        TPointPolygon getPosition(){return pos;}
        void setPosition(TPointPolygon pos_){pos = pos_;}
        bool operator < (const QPointFB &p) const{return this -> x() < p.x();}
};

#endif // QPOINTFB_H
