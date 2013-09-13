//
//  HMath.h
//  houyi
//
//  Created by Lance Mao on 12/08/2011.
//  Copyright 2011 Dong wen. All rights reserved.
//

#ifndef HMATH_H
#define HMATH_H

#include "HVector.h"
#include <cmath>

#define PI 3.141592

class HMath
{
public:
    static const double degree2radianRatio;
    static const double radian2degreeRatio;
    
    static inline double degree2radian(double degree)
    {
        return degree * degree2radianRatio;
    }
    
    static double radian2degree(double radian)
    {
        return radian * radian2degreeRatio;
    }
    
    static inline float abs(float v)
    {
        return fabsf(v);
    }
    
    static bool rayTriangleIntersect(HVector& res, HVector p, HVector d, HVector v0, HVector v1, HVector v2);
    
    static inline bool isAlmostEqual(double a, double b)
    {
        return (a - b < 0.00001);
    }
    
    static inline float min(float n1, float n2)
    {
        return n1 < n2 ? n1 : n2;
    }
    
    static inline float max(float n1, float n2)
    {
        return n1 > n2 ? n1 : n2;
    }
    
    static inline float max(float n1, float n2, float n3)
    {
        return n1 > n2 ? max(n1, n3) : max(n2, n3);
    }
    
    template<class T>
    static inline T clamp(const T& v, const T& min, const T& max)
    {
        return v < min ? min : (v > max ? max : v);
    }
};

#endif
