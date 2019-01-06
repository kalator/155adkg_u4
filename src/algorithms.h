#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QtGui>
#include <qpointfb.h>

class Algorithms
{
public:
    Algorithms() {}

    static TPointPolygon getPositionWinding(QPointFB q, std::vector<QPointFB> pol);
    static TPointLinePosition getPointLinePosition(QPointFB &q, QPointFB &a, QPointFB &b);
    static double get2LinesAngle(QPointFB &p1,QPointFB &p2,QPointFB &p3, QPointFB &p4);
    static T2LinesPosition get2LinesPosition(QPointFB &p1,QPointFB &p2,QPointFB &p3, QPointFB &p4, QPointFB &intersection);
    static void computePolygonIntersections(std::vector<QPointFB> &p1, std::vector<QPointFB> &p2);
    static void processIntersection(QPointFB &b, double t, std::vector<QPointFB> &poly, int &i);
    static void setPositions (std::vector<QPointFB> &pol1,std::vector<QPointFB> &pol2);
    static void createFragments(std::vector<QPointFB> &pol, TPointPolygon pos, bool swap, std::map <QPointFB, std::pair<bool,  std::vector<QPointFB> > > &fragments);
    static bool createFragmentFromVertices (int i_start, std::vector<QPointFB> &pol, TPointPolygon pos, int &i, std::vector<QPointFB> &fr);
    static void mergeFragments(std::map<QPointFB, std::pair<bool, std::vector<QPointFB> > > &FR, std::vector<std::vector<QPointFB> > &res);
    static bool createPolygonFromFragments(QPointFB &start, std::map<QPointFB, std::pair<bool, std::vector<QPointFB> > > &FR, std::vector<QPointFB>  &pol);
    static double getPolygonOrientation(std::vector<QPointFB> &pol);
    static std::vector<std::vector<QPointFB> > BooleanOper(std::vector<QPointFB> &A, std::vector<QPointFB> &B, TBooleanOperation oper);
    static void resetIntersections(std::vector<QPointFB> &A);
    static std::vector<QPointFB> lineOffset(QPointFB &p1,QPointFB &p2, double d, double delta);
//    static std::vector<std::vector<QPointFB> > lineOffset(std::vector<QPointFB> &pol, double d, double delta);
//    static void sampleArc(QPointFB &s, double r, double fi_s, double fi_e, double delta, std::vector<QPointFB> &pol);
//    static std::vector<std::vector<QPointFB> > polygonOffset(std::vector<QPointFB> &pol, double d, double delta);
};

#endif // ALGORITHMS_H
