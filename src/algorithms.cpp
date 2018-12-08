#include "algorithms.h"
#include <map>


int Algorithms::getPositionWinding(QPointFB q, std::vector<QPointFB> pol)
{
       int n = pol.size();
       double sum_fi=0.0;
       double eps = 1.0e-10;

       //Process all polygon segments
       for (int i=0; i < n; i++)
       {
           //Get angle
           double fi = get2LinesAngle(pol[i], q, pol[(i+1)%n], q);

           //Get position
           int t = getPointLinePosition(q, pol[i], pol[(i+1)%n]);

           //Point q in the left halfplane
           if(t > 0)
               sum_fi += fi;

           //Point in the right halfplane
           else
               sum_fi -= fi;
       }


       //Point q inside P
       if (fabs(fabs(sum_fi)-360)< eps)
           return 1;

       //Point q outside P
       if (fabs(sum_fi) < eps)
           return 0;

       //Point q on the boundary
       else
           return -1;
}


int Algorithms::getPointLinePosition(QPointFB &q, QPointFB &a, QPointFB &b)
{
    //Point and line position
    double eps = 1.0e-10;

    double ux = b.x() - a.x();
    double uy = b.y() - a.y();
    double vx = q.x() - a.x();
    double vy = q.y() - a.y();

    //Determinant
    double t = (ux * vy - uy * vx);

    //Point in the left half plane
    if(t > eps)
        return 1;

    //Point in the right half plane
    if(t < -eps)
        return 0;

    //Point on the line
    return -1;
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
    double eps = 10e-10;
    //Get Position of two lines and compute their intersection
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();

    double vx = p4.x() - p3.x();
    double vy = p4.y() - p3.y();

    double wx = p1.x() - p3.x();
    double wy = p1.y() - p3.y();

    double k1 = vx*wy - vy*wx;
    double k2 = ux*wy - uy*wx;
    double k3 = vy*ux - vx*uy;

    // lines are colinear
    if(fabs(k1)< eps && fabs(k2)< eps && fabs(k3)<eps)
    {
        return COLINEAR;
    }

    // lines are parallel
    if (fabs(k1) <eps && fabs(k2)< eps)
    {
        return PARALLEL;
    }

    // lines intersect each other
    double alfa = k1/k3;
    double beta = k2/k3;

    if(alfa >= 0 && alfa <= 1 && beta >= 0 && beta <= 1)
    {

        //Compute intersection
        double xq = p1.x() + alfa*ux;
        double yq = p1.y() + alfa*uy;

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
    for(unsigned int i = 0; i<p1.size(); i++)
    {
        // Map of intersection sorted by alpha
        std::map<double, QPointFB> inters;
        for(unsigned int j = 0;j<p2.size(); j++)
        {
            //Compute intersection
            QPointFB pi;
            if(get2LinesPosition(p1[i],p1[(i+1)%p1.size()],p2[j],p2[(j+1)%p2.size()], pi) == INTERSECTING)
            {
                //Add for later processing
                inters[pi.getAlfa()] = pi;

                //Intersection on pj
                if(pi.getBeta() == 0)
                {
                    p2[j].setInters(true);
                }

                //Intersection on pj+1
                else if(pi.getBeta() == 1)
                {
                    p2[(j+1)%p2.size()].setInters(true);
                }

                //Intersection at current point
                else
                {
                    p2.insert(p2.begin()+(j+1)%p2.size(), pi);
                    j++;
                }
            }
        }

        //All intersections (pi, pi+1) with p2
        if(!inters.empty()){
            for(auto pi2:inters){
                if(pi2.second.getAlfa() == 0)
                {
                    p1[i].setInters(true);
                }
                //Intersection on pj+1
                else if(pi2.second.getAlfa() == 1)
                {
                    p1[(i+1)%p1.size()].setInters(true);
                }

                //Intersection at current point
                else
                {
                    p1.insert(p1.begin()+(i+1)%p1.size(), pi2.second);
                    i++;
                }
            }
        }
    }
}
