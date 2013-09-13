//
//  HMatrix.h
//  houyi
//
//  Created by Lance Mao on 12/08/2011.
//  Copyright 2011 Dong wen. All rights reserved.
//

#ifndef __Houyi__HouyiMatrix__
#define __Houyi__HouyiMatrix__

#include "HVector.h"

class HMatrix
{
public:
    HMatrix();
    HMatrix(float data[16]);
    HMatrix& operator=(const HMatrix& m);
    
    float& operator[](const int index);
    const float& operator[](const int index) const;
    HMatrix operator*(const HMatrix& m) const;
    HMatrix& operator*=(const HMatrix& m);

    void makeIdentity();
    float* getData();

    HVector multiply(const HVector& v) const;
    HMatrix multiply(const HMatrix& m) const;
    void translate(const float tx, const float ty, const float tz);
    void rotate(const float theta, HVector& axis);
    void rotate(const float theta, float x, float y, float z);
    void scale(const float sx, const float sy, const float sz);
    void transpose();
    float determinant() const;
    HMatrix inverse() const;
    void ortho(float left, float right, float top, float bottom, float near, float far);
    
    static void makeIdentity(float* m);
    static void copyMatrix(float* res, const float* src);
    static HVector multiply(float* m, float x, float y, float z);
    static HVector multiply(float* m, const HVector& v);
    static void multiply(float* res, float* m1, float* m2);
    static void transpose(float* res, float* src);
    static void transpose(HMatrix& res, HMatrix& src);
    static float determinant(float* m);
    static bool inverse(float* i, float* m);
    
private:
    float mData[16];
};

#endif
