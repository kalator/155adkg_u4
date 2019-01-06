#include "algorithms.h"
#include <map>
#include <cmath>


TPointPolygon Algorithms::getPositionWinding(QPointFB q, std::vector<QPointFB> pol)
{
       int n = pol.size();
       double sum_fi=0.0;
       double eps = 1.0e-5;

       //Process all polygon segments
       for (int i=0; i < n; i++)
       {
           //Get angle
           double fi = get2LinesAngle(pol[i], q, pol[(i+1)%n], q);

           //Get position
           int t = getPointLinePosition(q, pol[i], pol[(i+1)%n]);

           //Point on the boundary
           if (t == COL && fabs(fabs(fi) - 180) < eps)
               return ON;

           //Point q in the left halfplane
           if(t > 0)
               sum_fi += fi;

           //Point in the right halfplane
           else
               sum_fi -= fi;
       }

       //Point q inside P
       if (fabs(fabs(sum_fi)-360)< eps)
           return INSIDE;

       //Point q outside P
       if (fabs(sum_fi) < eps)
           return OUTSIDE;
}


TPointLinePosition Algorithms::getPointLinePosition(QPointFB &q, QPointFB &a, QPointFB &b)
{
    //Point and line position
    double eps = 1.0e-8;

    double ux = b.x() - a.x();
    double uy = b.y() - a.y();
    double vx = q.x() - a.x();
    double vy = q.y() - a.y();

    //Determinant
    double t = (ux * vy - uy * vx);

    //Point in the left half plane
    if(t > eps)
        return LEFT;

    //Point in the right half plane
    if(t < -eps)
        return RIGHT;

    //Point on the line
    return COL;
}


double Algorithms::get2LinesAngle(QPointFB &p1,QPointFB &p2,QPointFB &p3, QPointFB &p4)
{
    //Angle between two lines
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double vx = p4.x() - p3.x();
    double vy = p4.y() - p3.y();

    //Dot product
    double dot = ux * vx + uy * vy;

    //Norms
    double nu = sqrt(ux * ux + uy * uy);
    double nv = sqrt(vx * vx + vy * vy);

    return fabs(acos(dot/(nu * nv)))*(180/M_PI);
}


T2LinesPosition Algorithms::get2LinesPosition(QPointFB &p1,QPointFB &p2,QPointFB &p3, QPointFB &p4, QPointFB &intersection)
{
    //Get position of two lines and compute their intersection
    double eps = 10e-10;

    //Coordinate differences
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();

    double vx = p4.x() - p3.x();
    double vy = p4.y() - p3.y();

    double wx = p1.x() - p3.x();
    double wy = p1.y() - p3.y();

    //Compute k1 - k3
    double k1 = vx*wy - vy*wx;
    double k2 = ux*wy - uy*wx;
    double k3 = vy*ux - vx*uy;

    // lines are colinear
    if(fabs(k1)< eps && fabs(k2)< eps && fabs(k3)<eps)
    {
        return COLINEAR;
    }

    // lines are parallel
    if (fabs(k1) < eps && fabs(k2) < eps)
    {
        return PARALLEL;
    }

    // lines intersect each other
    double alfa = k1/k3;
    double beta = k2/k3;

    if(alfa >= 0 && alfa <= 1 && beta >= 0 && beta <= 1)
    {

        //Compute intersection
        double xq = p1.x() + alfa * ux;
        double yq = p1.y() + alfa * uy;

        intersection.setX(xq);
        intersection.setY(yq);

        intersection.setAlfa(alfa);
        intersection.setBeta(beta);
        intersection.setInters(true);

        return INTERSECTING;

    }
    return NONINTERSECTING;
}


void Algorithms::computePolygonIntersections(std::vector<QPointFB> &p1, std::vector<QPointFB> &p2)
{
    //Compute intersections of two polygons and update the list
    for(int i = 0; i < p1.size(); i++)
    {
        // Map of intersection sorted by alpha
        std::map<double, QPointFB> inters;
        for(int j = 0;j < p2.size(); j++)
        {
            //Compute intersection
            QPointFB pi;
            if(get2LinesPosition(p1[i], p1[(i+1)%p1.size()], p2[j],p2[(j+1)%p2.size()], pi) == INTERSECTING)
            {
                //Add for later processing
                inters[pi.getAlfa()] = pi;

                // Process intersection
                processIntersection(pi, pi.getBeta(), p2, j);
            }
        }

        //All intersections (pi, pi+1) with p2
        if(!inters.empty()){
            for(auto pi2:inters)
            {
                // Process intersection
                processIntersection(pi2.second, pi2.second.getAlfa(), p1, i);
            }
        }
    }
}


void Algorithms::processIntersection(QPointFB &b, double t, std::vector<QPointFB> &poly, int &i)
{
    //Process intersection
    double eps = 1e-10;

    //Intersection on p[i]
    if(fabs(t) < eps)
    {
        poly[i].setInters(true);
    }

    //Intersection on p[i+1]
    else if(fabs(fabs(t)-1) < eps)
    {
        poly[(i+1)%poly.size()].setInters(true);
    }

    //Intersection at current point
    else
    {
        i++;
        poly.insert(poly.begin() + i, b);
    }
}


void Algorithms::setPositions (std::vector<QPointFB> &pol1,std::vector<QPointFB> &pol2)
{
    //Set positions of midpoints of edges
    //Process first polygon
    for (int i = 0; i < pol1.size(); i++)
    {
        QPointFB m((pol1[i].x() + pol1[(i+1)%pol1.size()].x())*0.5, (pol1[i].y() + pol1[(i+1)%pol1.size()].y())*0.5);
        TPointPolygon loc= getPositionWinding(m,pol2);
        pol1[i].setPosition(loc);
    }

    //Process second polygon
    for (int i = 0; i < pol2.size(); i++)
    {
        QPointFB m((pol2[i].x() + pol2[(i+1)%pol2.size()].x())*0.5, (pol2[i].y() + pol2[(i+1)%pol2.size()].y())*0.5);
        TPointPolygon loc= getPositionWinding(m,pol1);
        pol2[i].setPosition(loc);
    }
}

void Algorithms::createFragments(std::vector<QPointFB> &pol, TPointPolygon pos, bool swap, std::map <QPointFB, std::pair<bool, std::vector<QPointFB>> > &fragments)
{
    //Select fragments by the position and order them according to the swap
    int i = pol.size() - 1, i_start = -1;
    for (; i > 0; i--)
        if (pol[i].getInters() && pol[i].getPosition() == pos)
            i_start = i;

    //There is no vertex with this position
    if (i_start < 0 )
       return;

    //Remeber start index
    i = i_start;
    do
    {
        //Start point of the fragment has been found
        if (pol[i].getInters() && (pol[i].getPosition() == pos))
        {
            std::vector<QPointFB> fr;
            if (createFragmentFromVertices (i_start, pol, pos, i, fr) && fr.size() > 1)
            {
                 //Change order
                 if(swap) std::reverse(fr.begin(), fr.end());

                 //Add to the map
                 fragments[fr[0]] = {false, fr};
            }
        }

        //Increment i
        else i = (i + 1) % pol.size();

    } while (i != i_start);
}


bool Algorithms::createFragmentFromVertices (int i_start, std::vector<QPointFB> &pol, TPointPolygon pos, int &i, std::vector<QPointFB> &fr)
{
    //Create fragment from vertices with a given position
    for (;;)
    {
        //Add point
        fr.push_back(pol[i]);

        //Increment point index
        i = (i + 1) % pol.size();

        //We reached the start point again: error
        if (i == i_start)
            return false;

        //We found the last point with the given orientation
        if (pol[i].getPosition() != pos)
        {
            //Add last point with a different orientation (next intersection)
            fr.push_back(pol[i]);

            return true;
        }
    }
}


void Algorithms::mergeFragments(std::map<QPointFB, std::pair<bool, std::vector<QPointFB> > > &FR, std::vector<std::vector<QPointFB> > &res)
{
    //Merge fragments to the results
    for (auto fr: FR)
    {
        //Resulting polygon
        std::vector<QPointFB> pol;

        //Create polygon from fragments
        QPointFB s = fr.first;

        //Fragment not processed
        if (!fr.second.first)
        {
            //Polygon has been found
            if (createPolygonFromFragments(s, FR, pol))
            {
                if (pol.size() > 2 && fabs(getPolygonOrientation(pol)) > 0.1)
                    res.push_back(pol);
            }
        }
    }
}

bool Algorithms::createPolygonFromFragments(QPointFB &start, std::map<QPointFB, std::pair<bool, std::vector<QPointFB> > > &FR, std::vector<QPointFB>  &pol)
{
    //Create polygon from fragments
    QPointFB next = start;

    for(;;)
    {
            //Find next fragment
            auto fr = FR.find(next);

            //Next fragment does not exist
            if (fr == FR.end())
                return false;

            //Fragment has already been processed
            //if (fr->second.first)
            //    return false;

            //Set fragment as processed
            fr->second.first = true;

            //Find new next point
            next= fr->second.second.back();

            //Add to the polygon
            pol.insert(pol.end(), fr->second.second.begin() + 1, fr->second.second.end());

            //Both points are analogous
            if (next == start)
                return true;
    }
}

double Algorithms::getPolygonOrientation(std::vector<QPointFB> &pol){

    // Get polygon orientation using LH formula
    double A = 0;
    for(int i = 0; i < pol.size(); i++)
    {
        A += pol[i].x()*(pol[(i + 1)%pol.size()].y() - pol[(i - 1 + pol.size())%pol.size()].y());
    }

    return 0.5 * A;
}


std::vector<std::vector<QPointFB> > Algorithms::BooleanOper(std::vector<QPointFB> &A, std::vector<QPointFB> &B, TBooleanOperation oper)
{
    //Perform boolean operation of polygons

    //Switch orientation of polygon A
    if (getPolygonOrientation(A) < 0)
    {
        reverse (A.begin(), A.end());
    }

    //Switch orientation of polygon B
    if (getPolygonOrientation(B) < 0)
    {
        reverse (B.begin(), B.end());
    }

    //Compute Intersection
    computePolygonIntersections(A, B);

    //Set Positions
    setPositions(A,B);

    //Create map of fragments
    std::map<QPointFB, std::pair<bool, std::vector<QPointFB> > > F;

    //Set type of vertices
    TPointPolygon pos1 = (oper == INTERSECTION || oper == DIFFBA? INSIDE : OUTSIDE);
    TPointPolygon pos2 = (oper == INTERSECTION || oper == DIFFAB ? INSIDE : OUTSIDE);
    bool rever1 = (oper == DIFFAB ? true : false);
    bool rever2 = (oper == DIFFBA ? true : false);

    //Create fragments
    createFragments(A,pos1, rever1, F);
    createFragments(A, ON, rever1, F);
    createFragments(B,pos2, rever2, F);
    createFragments(B, ON, rever1, F);

    //Merge fragments
    std::vector<std::vector<QPointFB> > res;
    mergeFragments(F, res);

    //Reset intersections
    for (int i = 0; i < res.size();i++)
       resetIntersections(res[i]);

    resetIntersections(A);
    resetIntersections(B);
       //for (int j = 0; j < res[i].size();j++)
       //         res[i][j].setInters(false);
    //for (int i = 0; i < A.size();i++)
    //    A[i].setInters(false);
    //    for (int i = 0; i < B.size();i++)
    //        B[i].setInters(false);

    return res;
}


void Algorithms::resetIntersections(std::vector<QPointFB> &pol)
{
    //Reset intersections
    for (int i = 0; i < pol.size();i++)
    {
             pol[i].setInters(false);
    }
}

/*std::vector<QPointFB> Algorithms::lineOffset(QPointFB &p1,QPointFB &p2, double d, double delta)
{
    //Offset of the line segment
    std::vector<QPointFB> C;

    //Direction of the line
    double fi = atan2(p2.y()-p1.y(),p2.x()-p1.x());

    //Starting arc
    double fi_s1 = fi + M_PI/2;
    double fi_e1 = fi_s1 + M_PI;

    //End arc
    double fi_s2 = fi - M_PI/2;
    double fi_e2 = fi_s2 + M_PI;

    //Create points on the arcs
    sampleArc(p1, d, fi_s1, fi_e1, delta, C);
    sampleArc(p2, d, fi_s2, fi_e2, delta, C);

    return C;
}

void Algorithms::sampleArc(QPointFB &s, double r, double fi_s, double fi_e, double delta, std::vector<QPointFB> &pol)
{
   //Sampling arc with the given step
    double eps = 10e-10;

    QPointFB p1(s.x()+r*cos(fi_s), s.y()+r*sin(fi_s));
    pol.push_back(p1);

    //Find integer multiplier
    double alfa = fi_s/delta;
    alfa = (fabs(alfa-ceil(alfa)) < eps ? alfa+1 : ceil(alfa));

    //Sample all internal points of arc
    for(double fi = alfa*delta; fi < fi_e; fi += delta)
    {
        QPointFB q(s.x()+r*cos(fi), s.y()+r*sin(fi));
        pol.push_back(q);
    }

    //Add end point
    QPointFB p2(s.x()+r*cos(fi_e), s.y()+r*sin(fi_e));
    pol.push_back(p2);
}

std::vector<std::vector<QPointFB> > Algorithms::lineOffset(std::vector<QPointFB> &pol, double d, double delta)
{
    //Create buffer over polygon
    std::vector<std::vector<QPointFB> > buff;
    for(int i = 0; i < pol.size(); i++)
    {
        buff.push_back(lineOffset(pol[i], pol[(i+1)%pol.size()], d, delta));
    }

    return buff;
}

std::vector<std::vector<QPointFB> > Algorithms::polygonOffset(std::vector<QPointFB> &pol, double d, double delta)
{
    //Create buffer over polygons
    std::vector<std::vector<QPointFB> > buff;

    //Create buffer over the first edge
    std::vector<QPointFB> buff_0 = lineOffset(pol[0], pol[1], d, delta);
    buff.push_back(buff_0);

    //Create buffer over each edge
    for(int i = 1; i < pol.size(); i++)
    {
        //Create buffer over each edge
        std::vector<QPointFB> b = lineOffset(pol[i], pol[(i+1)%pol.size()], d, delta);

        //Disolve for all segments of all segments
        std::vector<std::vector<QPointFB> > buff_h;

        //Process all segments of current buffer and do union
        for (int j=0; j<buff.size(); j++){

            //union of the line buffer and current buffer
            TBooleanOperation uni = UNION;
            std::vector<std::vector<QPointFB> > u = BooleanOper(b,buff[j],uni);

            //union is empty -> add inner buffer
            if (u.size() == 0)
            {
                buff_h.push_back(buff[j]);
            }

            //update segments in new buffer
            else
            {
                buff_h.insert(buff_h.end(), u.begin(), u.end());
            }
        }

        buff = buff_h;
    }

    return buff;
}*/
