//
//  HMath.cpp
//  houyi
//
//  Created by Lance Mao on 12/08/2011.
//  Copyright 2011 Dong wen. All rights reserved.
//

#include "HMath.h"

const double HMath::degree2radianRatio = PI / 180;
const double HMath::radian2degreeRatio = 180 / PI;

bool HMath::rayTriangleIntersect(HVector& res, HVector p, HVector d, HVector v0, HVector v1, HVector v2)
{
    float a,f,u,v;
    HVector e1 = v1 - v0;
    HVector e2 = v2 - v0;
    
    HVector h = d.crossProduct(e2);
    a = e1.dotProduct(h);
    
    if (a > -0.00001 && a < 0.00001)
        return(false);
    
    f = 1/a;
    HVector s = p - v0;
    u = f * (s.dotProduct(h));
    
    if (u < 0.0 || u > 1.0)
        return(false);
    
    HVector q = s.crossProduct(e1);
    v = f * d.dotProduct(q);
    
    if (v < 0.0 || u + v > 1.0)
        return(false);
    
    // at this stage we can compute t to find out where
    // the intersection point is on the line
    float t = f * e2.dotProduct(q);
    
    if (t > 0.00001) // ray intersection
    {
        res = HVector(t, u, v);
        return (true);
    }
    else // this means that there is a line intersection
         // but not a ray intersection
    {
        return (false);
    }
}
