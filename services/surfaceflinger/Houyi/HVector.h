//
//  HVector.h
//  houyi
//
//  Created by Lance Mao on 12/08/2011.
//  Copyright 2011 Dong wen. All rights reserved.
//

#ifndef HVECTOR
#define HVECTOR

#include <math.h>

class HMatrix;

class HVector
{  
public:
    HVector() : x(0), y(0), z(0), w(0) {}
    HVector(float ax, float ay, float az) : x(ax), y(ay), z(az), w(0) {}
    
    inline HVector add(const HVector& v) const
    {
        return HVector(x + v.x, y + v.y, z + v.z);
    }
    
    inline HVector operator+(const HVector& v) const
    {
        return HVector(x + v.x, y + v.y, z + v.z);
    }
    
    inline HVector substract(const HVector& v) const
    {
        return HVector(x - v.x, y - v.y, z - v.z);
    }
    
    inline HVector operator-(const HVector& v) const
    {
        return HVector(x - v.x, y - v.y, z - v.z);
    }
    
    inline HVector& operator=(const HVector& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }
    
    inline HVector& operator+=(const HVector& v)
    {
        x+=v.x;
        y+=v.y;
        z+=v.z;
        return *this;
    }
    
    inline HVector& operator+=(float factor)
    {
        x+=factor;
        y+=factor;
        z+=factor;
        return *this;
    }
    
    inline HVector& operator-=(const HVector& v)
    {
        x-=v.x;
        y-=v.y;
        z-=v.z;
        return *this;
    }
    
    inline HVector& operator-=(float factor)
    {
        x-=factor;
        y-=factor;
        z-=factor;
        return *this;
    }
    
    inline HVector multiply(float factor) const
    {
        return HVector(x * factor, y * factor, z * factor);
    }
    
    inline HVector operator*(float factor) const
    {
        return HVector(x * factor, y * factor, z * factor);
    }
    
    inline HVector& operator*=(float factor)
    {
        x*=factor;
        y*=factor;
        z*=factor;
        return *this;
    }
    
    inline HVector divide(float factor) const
    {
        if (factor == 0) {
            return *this;
        }
        
        return HVector(x / factor, y / factor, z / factor);
    }
    
    inline HVector operator/(float factor) const
    {
        if (factor == 0) {
            return *this;
        }
        
        return HVector(x / factor, y / factor, z / factor);
    }
    
    inline HVector& operator/=(float factor)
    {
        x/=factor;
        y/=factor;
        z/=factor;
        return *this;
    }
    
    inline double magnitude() const
    {
        return sqrt(x * x + y * y + z * z);    
    }
    
    inline HVector& normalize()
    {
        double mag = magnitude();
        if (mag > -0.000001 && mag < 0.000001){
            return *this = HVector(0, 0, 0);
        } else {
            return *this /= mag;
        }
    }
    
    inline HVector normalize() const
    {
        HVector res = *this;
        return res.normalize();
    }
    
    inline float dotProduct(HVector v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }
    
    inline HVector crossProduct(const HVector& v) const
    {
        HVector vector;
        
        vector.x = y * v.z - z * v.y;
        vector.y = z * v.x - x * v.z;
        vector.z = x * v.y - y * v.x;
        
        return vector;
    }
    
    inline HVector& rotate(const HVector& along, double angle)
    {
        double x = (*this).x;
        double y = (*this).y;
        double z = (*this).z;
        double u = along.x;
        double v = along.y;
        double w = along.z;
        
        double ux = u * x;
        double uy = u * y;
        double uz = u * z;
        double vx = v * x;
        double vy = v * y;
        double vz = v * z;
        double wx = w * x;
        double wy = w * y;
        double wz = w * z;
        double sa = sin(angle);
        double ca = cos(angle);
        (*this).x = (float) (u * (ux + vy + wz)
                             + (x * (v * v + w * w) - u * (vy + wz)) * ca + (-wy + vz) * sa);
        (*this).y = (float) (v * (ux + vy + wz)
                             + (y * (u * u + w * w) - v * (ux + wz)) * ca + (wx - uz) * sa);
        (*this).z = (float) (w * (ux + vy + wz)
                             + (z * (u * u + v * v) - w * (ux + vy)) * ca + (-vx + uy) * sa);
        
        return *this;
    }
    
    inline HVector& rotate(const HVector& along, double angle) const
    {
        HVector res = *this;
        return res.rotate(along, angle);
    }
    
    HVector operator*(const HMatrix& matrix) const;
    HVector& operator*=(const HMatrix& matrix);
    
public:
    float x;
    float y;
    float z;
    float w;
    
    static HVector Zero;
    static HVector BasisX;
    static HVector BasisY;
    static HVector BasisZ;
};

#endif