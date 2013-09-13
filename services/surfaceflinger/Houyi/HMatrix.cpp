//
//  HMatrix.cpp
//  houyi
//
//  Created by Lance Mao on 12/08/2011.
//  Copyright 2011 Dong wen. All rights reserved.
//

#include "HMatrix.h"

HMatrix::HMatrix()
{
    makeIdentity();
}

HMatrix::HMatrix(float data[16])
{
    for (int i = 0;i < 16;i++)
    {
        mData[i] = data[i];
    }
}

HMatrix& HMatrix::operator=(const HMatrix& m)
{
	if (this != &m)
	{
		for (int i = 0;i < 16;i++)
		{
			mData[i] = m[i];
		}
	}
	return *this;
}

void HMatrix::makeIdentity()
{
    mData[0] = 1;mData[4] = 0;mData[8] = 0;mData[12] = 0;
    mData[1] = 0;mData[5] = 1;mData[9] = 0;mData[13] = 0;
    mData[2] = 0;mData[6] = 0;mData[10]= 1;mData[14] = 0;
    mData[3] = 0;mData[7] = 0;mData[11]= 0;mData[15] = 1;
}

float& HMatrix::operator[](int index)
{
    return mData[index];
}

const float& HMatrix::operator[](int index) const
{
    return mData[index];
}

HVector HMatrix::multiply(const HVector& v) const
{
    float w = mData[3] + mData[7] + mData[11] + mData[15];
    if (w == 0){
        return v;
    }
    
    return HVector((mData[0] * v.x + mData[4] * v.y + mData[8] * v.z + mData[12]) / w,
                   (mData[1] * v.x + mData[5] * v.y + mData[9] * v.z + mData[13]) / w,
                   (mData[2] * v.x + mData[6] * v.y + mData[10]* v.z + mData[14]) / w);
}

HMatrix HMatrix::multiply(const HMatrix& m) const
{
    HMatrix res;
    res.mData[0] = mData[0] * m.mData[0] + mData[4] * m.mData[1] + mData[8] * m.mData[2] + mData[12] * m.mData[3];
    res.mData[1] = mData[1] * m.mData[0] + mData[5] * m.mData[1] + mData[9] * m.mData[2] + mData[13] * m.mData[3];
    res.mData[2] = mData[2] * m.mData[0] + mData[6] * m.mData[1] + mData[10]* m.mData[2] + mData[14] * m.mData[3];
    res.mData[3] = mData[3] * m.mData[0] + mData[7] * m.mData[1] + mData[11]* m.mData[2] + mData[15] * m.mData[3];
    res.mData[4] = mData[0] * m.mData[4] + mData[4] * m.mData[5] + mData[8] * m.mData[6] + mData[12] * m.mData[7];
    res.mData[5] = mData[1] * m.mData[4] + mData[5] * m.mData[5] + mData[9] * m.mData[6] + mData[13] * m.mData[7];
    res.mData[6] = mData[2] * m.mData[4] + mData[6] * m.mData[5] + mData[10]* m.mData[6] + mData[14] * m.mData[7];
    res.mData[7] = mData[3] * m.mData[4] + mData[7] * m.mData[5] + mData[11]* m.mData[6] + mData[15] * m.mData[7];
    res.mData[8] = mData[0] * m.mData[8] + mData[4] * m.mData[9] + mData[8] * m.mData[10]+ mData[12] * m.mData[11];
    res.mData[9] = mData[1] * m.mData[8] + mData[5] * m.mData[9] + mData[9] * m.mData[10]+ mData[13] * m.mData[11];
    res.mData[10]= mData[2] * m.mData[8] + mData[6] * m.mData[9] + mData[10]* m.mData[10]+ mData[14] * m.mData[11];
    res.mData[11]= mData[3] * m.mData[8] + mData[7] * m.mData[9] + mData[11]* m.mData[10]+ mData[15] * m.mData[11];
    res.mData[12]= mData[0] * m.mData[12]+ mData[4] * m.mData[13]+ mData[8] * m.mData[14]+ mData[12] * m.mData[15];
    res.mData[13]= mData[1] * m.mData[12]+ mData[5] * m.mData[13]+ mData[9] * m.mData[14]+ mData[13] * m.mData[15];
    res.mData[14]= mData[2] * m.mData[12]+ mData[6] * m.mData[13]+ mData[10]* m.mData[14]+ mData[14] * m.mData[15];
    res.mData[15]= mData[3] * m.mData[12]+ mData[7] * m.mData[13]+ mData[11]* m.mData[14]+ mData[15] * m.mData[15];
    return res;
}

void HMatrix::translate(const float tx, const float ty, const float tz)
{
    HMatrix t;
    t[12] = tx;
    t[13] = ty;
    t[14] = tz;
    (*this) = t * (*this);
}

void HMatrix::rotate(const float theta, HVector& axis)
{
	rotate(theta, axis.x, axis.y, axis.z);
}

void HMatrix::rotate(const float theta, float x, float y, float z)
{
	if (theta == 0) {
		return;
	}

	/*
	 * This function performs an axis/angle rotation. (x,y,z) is any vector
	 * on the axis. For greater speed, always use a unit vector (length =
	 * 1). In this version, we will assume an arbitrary length and
	 * normalize.
	 */

	double length;
	double c, s, t;

	// normalize
	length = sqrt(x * x + y * y + z * z);

	// too close to 0, can't make a normalized vector
	if (length < .000001)
		return;

	x /= length;
	y /= length;
	z /= length;

	// do the trig
	c = cos(theta);
	s = sin(theta);
	t = 1 - c;

	// build the rotation matrix
	float ViewRotationMatrix[16];
	ViewRotationMatrix[0] = (float) (t * x * x + c);
	ViewRotationMatrix[1] = (float) (t * x * y - s * z);
	ViewRotationMatrix[2] = (float) (t * x * z + s * y);
	ViewRotationMatrix[3] = 0;

	ViewRotationMatrix[4] = (float) (t * x * y + s * z);
	ViewRotationMatrix[5] = (float) (t * y * y + c);
	ViewRotationMatrix[6] = (float) (t * y * z - s * x);
	ViewRotationMatrix[7] = 0;

	ViewRotationMatrix[8] = (float) (t * x * z - s * y);
	ViewRotationMatrix[9] = (float) (t * y * z + s * x);
	ViewRotationMatrix[10] = (float) (t * z * z + c);
	ViewRotationMatrix[11] = 0;

	ViewRotationMatrix[12] = 0;
	ViewRotationMatrix[13] = 0;
	ViewRotationMatrix[14] = 0;
	ViewRotationMatrix[15] = 1;

	// build the transform
	multiply(mData, ViewRotationMatrix, mData);
}

void HMatrix::scale(const float sx, const float sy, const float sz)
{
    mData[0] *= sx;
    mData[5] *= sy;
    mData[10] *= sz;
}

void HMatrix::transpose()
{
    float res[16];
    HMatrix::transpose(res, mData);
    HMatrix::copyMatrix(mData, res);
}

HMatrix HMatrix::operator*(const HMatrix& m) const
{
    return this->multiply(m);
}

HMatrix& HMatrix::operator*=(const HMatrix& m)
{
    HMatrix res = this->multiply(m);
    return (*this) = res;
}

float HMatrix::determinant() const
{
    return mData[12] * mData[9] * mData[6] * mData[3] - mData[8] * mData[13] * mData[6] * mData[3] - mData[12]
    * mData[5] * mData[10] * mData[3] + mData[4] * mData[13] * mData[10] * mData[3] + mData[8]
    * mData[5] * mData[14] * mData[3] - mData[4] * mData[9] * mData[14] * mData[3] - mData[12]
    * mData[9] * mData[2] * mData[7] + mData[8] * mData[13] * mData[2] * mData[7] + mData[12]
    * mData[1] * mData[10] * mData[7] - mData[0] * mData[13] * mData[10] * mData[7] - mData[8]
    * mData[1] * mData[14] * mData[7] + mData[0] * mData[9] * mData[14] * mData[7] + mData[12]
    * mData[5] * mData[2] * mData[11] - mData[4] * mData[13] * mData[2] * mData[11] - mData[12]
    * mData[1] * mData[6] * mData[11] + mData[0] * mData[13] * mData[6] * mData[11] + mData[4]
    * mData[1] * mData[14] * mData[11] - mData[0] * mData[5] * mData[14] * mData[11] - mData[8]
    * mData[5] * mData[2] * mData[15] + mData[4] * mData[9] * mData[2] * mData[15] + mData[8]
    * mData[1] * mData[6] * mData[15] - mData[0] * mData[9] * mData[6] * mData[15] - mData[4]
    * mData[1] * mData[10] * mData[15] + mData[0] * mData[5] * mData[10] * mData[15];
}

HMatrix HMatrix::inverse() const
{
    float x = determinant();
    if (x == 0)
        return *this;
    
    HMatrix i;
    const float* m = mData;
    i.mData[0] = (-m[13] * m[10] * m[7] + m[9] * m[14] * m[7] + m[13] * m[6]
            * m[11] - m[5] * m[14] * m[11] - m[9] * m[6] * m[15] + m[5]
            * m[10] * m[15]) / x;
    i.mData[4] = (m[12] * m[10] * m[7] - m[8] * m[14] * m[7] - m[12] * m[6]
            * m[11] + m[4] * m[14] * m[11] + m[8] * m[6] * m[15] - m[4]
            * m[10] * m[15]) / x;
    i.mData[8] = (-m[12] * m[9] * m[7] + m[8] * m[13] * m[7] + m[12] * m[5]
            * m[11] - m[4] * m[13] * m[11] - m[8] * m[5] * m[15] + m[4]
            * m[9] * m[15]) / x;
    i.mData[12] = (m[12] * m[9] * m[6] - m[8] * m[13] * m[6] - m[12] * m[5]
             * m[10] + m[4] * m[13] * m[10] + m[8] * m[5] * m[14] - m[4]
             * m[9] * m[14]) / x;
    i.mData[1] = (m[13] * m[10] * m[3] - m[9] * m[14] * m[3] - m[13] * m[2]
            * m[11] + m[1] * m[14] * m[11] + m[9] * m[2] * m[15] - m[1]
            * m[10] * m[15]) / x;
    i.mData[5] = (-m[12] * m[10] * m[3] + m[8] * m[14] * m[3] + m[12] * m[2]
            * m[11] - m[0] * m[14] * m[11] - m[8] * m[2] * m[15] + m[0]
            * m[10] * m[15]) / x;
    i.mData[9] = (m[12] * m[9] * m[3] - m[8] * m[13] * m[3] - m[12] * m[1]
            * m[11] + m[0] * m[13] * m[11] + m[8] * m[1] * m[15] - m[0]
            * m[9] * m[15]) / x;
    i.mData[13] = (-m[12] * m[9] * m[2] + m[8] * m[13] * m[2] + m[12] * m[1]
             * m[10] - m[0] * m[13] * m[10] - m[8] * m[1] * m[14] + m[0]
             * m[9] * m[14]) / x;
    i.mData[2] = (-m[13] * m[6] * m[3] + m[5] * m[14] * m[3] + m[13] * m[2]
            * m[7] - m[1] * m[14] * m[7] - m[5] * m[2] * m[15] + m[1]
            * m[6] * m[15]) / x;
    i.mData[6] = (m[12] * m[6] * m[3] - m[4] * m[14] * m[3] - m[12] * m[2] * m[7]
            + m[0] * m[14] * m[7] + m[4] * m[2] * m[15] - m[0] * m[6]
            * m[15]) / x;
    i.mData[10] = (-m[12] * m[5] * m[3] + m[4] * m[13] * m[3] + m[12] * m[1]
             * m[7] - m[0] * m[13] * m[7] - m[4] * m[1] * m[15] + m[0]
             * m[5] * m[15]) / x;
    i.mData[14] = (m[12] * m[5] * m[2] - m[4] * m[13] * m[2] - m[12] * m[1]
             * m[6] + m[0] * m[13] * m[6] + m[4] * m[1] * m[14] - m[0]
             * m[5] * m[14]) / x;
    i.mData[3] = (m[9] * m[6] * m[3] - m[5] * m[10] * m[3] - m[9] * m[2] * m[7]
            + m[1] * m[10] * m[7] + m[5] * m[2] * m[11] - m[1] * m[6]
            * m[11]) / x;
    i.mData[7] = (-m[8] * m[6] * m[3] + m[4] * m[10] * m[3] + m[8] * m[2] * m[7]
            - m[0] * m[10] * m[7] - m[4] * m[2] * m[11] + m[0] * m[6]
            * m[11]) / x;
    i.mData[11] = (m[8] * m[5] * m[3] - m[4] * m[9] * m[3] - m[8] * m[1] * m[7]
             + m[0] * m[9] * m[7] + m[4] * m[1] * m[11] - m[0] * m[5]
             * m[11]) / x;
    i.mData[15] = (-m[8] * m[5] * m[2] + m[4] * m[9] * m[2] + m[8] * m[1] * m[6]
             - m[0] * m[9] * m[6] - m[4] * m[1] * m[10] + m[0] * m[5]
             * m[10]) / x;
    
    return i;
}

void HMatrix::ortho(float left, float right, float top, float bottom, float near, float far)
{
    mData[0] = 2 / (right - left);
    mData[1] = 0;
    mData[2] = 0;
    mData[3] = 0;

    mData[4] = 0;
    mData[5] = 2 / (top - bottom);
    mData[6] = 0;
    mData[7] = 0;

    mData[8] = 0;
    mData[9] = 0;
    mData[10] = 0;
    mData[11] = 0;

    mData[12] = -(right + left) / (right - left);
    mData[13] = -(top + bottom) / (top - bottom);
    mData[14] = 0;
    mData[15] = 1;
}

float* HMatrix::getData()
{
	return mData;
}

void HMatrix::makeIdentity(float* m)
{
    m[0] = 1;m[4] = 0;m[8] = 0;m[12] = 0;
    m[1] = 0;m[5] = 1;m[9] = 0;m[13] = 0;
    m[2] = 0;m[6] = 0;m[10] = 1;m[14] = 0;
    m[3] = 0;m[7] = 0;m[11] = 0;m[15] = 1;
}

void HMatrix::copyMatrix(float* res, const float* src)
{
    if (res && src)
    {
        for (int i = 0;i < 16;++i)
        {
            res[i] = src[i];
        }
    }
}

HVector HMatrix::multiply(float* m, float x, float y, float z)
{
    float w = m[3] * x + m[7] * y + m[11] * z + m[15];
    if (w == 0){
        return HVector(0, 0, 0);
    }
    
    return HVector((m[0] * x + m[4] * y + m[8] * z + m[12]) / w,
                       (m[1] * x + m[5] * y + m[9] * z + m[13]) / w,
                       (m[2] * x + m[6] * y + m[10] * z + m[14]) / w);
}

HVector HMatrix::multiply(float* m, const HVector& v)
{
    return HMatrix::multiply(m, v.x, v.y, v.z);
}

void HMatrix::multiply(float* res, float* m1, float* m2)
{
    if (res == m1 || res == m2)
    {
        float tmp[16];
        tmp[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
        tmp[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
        tmp[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
        tmp[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];
        tmp[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
        tmp[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
        tmp[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
        tmp[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];
        tmp[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
        tmp[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
        tmp[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
        tmp[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];
        tmp[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
        tmp[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
        tmp[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
        tmp[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];
        
        for (int i = 0;i < 16;++i)
        {
            res[i] = tmp[i];
        }
    }
    else
    {
        res[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
        res[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
        res[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
        res[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];
        res[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
        res[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
        res[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
        res[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];
        res[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
        res[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
        res[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
        res[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];
        res[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
        res[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
        res[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
        res[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];
    }
}

void HMatrix::transpose(float* res, float* src)
{
    for (int i = 0;i < 16;i++)
    {
        int srcRow = i / 4;
        int srcCol = i % 4;
        res[srcCol * 4 + srcRow] = src[i];
    }
}

void HMatrix::transpose(HMatrix& res, HMatrix& src)
{
    transpose(res.mData, src.mData);
}

float HMatrix::determinant(float* m)
{
    return m[12] * m[9] * m[6] * m[3] - m[8] * m[13] * m[6] * m[3] - m[12]
    * m[5] * m[10] * m[3] + m[4] * m[13] * m[10] * m[3] + m[8]
    * m[5] * m[14] * m[3] - m[4] * m[9] * m[14] * m[3] - m[12]
    * m[9] * m[2] * m[7] + m[8] * m[13] * m[2] * m[7] + m[12]
    * m[1] * m[10] * m[7] - m[0] * m[13] * m[10] * m[7] - m[8]
    * m[1] * m[14] * m[7] + m[0] * m[9] * m[14] * m[7] + m[12]
    * m[5] * m[2] * m[11] - m[4] * m[13] * m[2] * m[11] - m[12]
    * m[1] * m[6] * m[11] + m[0] * m[13] * m[6] * m[11] + m[4]
    * m[1] * m[14] * m[11] - m[0] * m[5] * m[14] * m[11] - m[8]
    * m[5] * m[2] * m[15] + m[4] * m[9] * m[2] * m[15] + m[8]
    * m[1] * m[6] * m[15] - m[0] * m[9] * m[6] * m[15] - m[4]
    * m[1] * m[10] * m[15] + m[0] * m[5] * m[10] * m[15];
}

bool HMatrix::inverse(float* i, float* m)
{
    float x = determinant(m);
    if (x == 0)
        return false;
    
    i[0] = (-m[13] * m[10] * m[7] + m[9] * m[14] * m[7] + m[13] * m[6]
            * m[11] - m[5] * m[14] * m[11] - m[9] * m[6] * m[15] + m[5]
            * m[10] * m[15])
    / x;
    i[4] = (m[12] * m[10] * m[7] - m[8] * m[14] * m[7] - m[12] * m[6]
            * m[11] + m[4] * m[14] * m[11] + m[8] * m[6] * m[15] - m[4]
            * m[10] * m[15])
    / x;
    i[8] = (-m[12] * m[9] * m[7] + m[8] * m[13] * m[7] + m[12] * m[5]
            * m[11] - m[4] * m[13] * m[11] - m[8] * m[5] * m[15] + m[4]
            * m[9] * m[15])
    / x;
    i[12] = (m[12] * m[9] * m[6] - m[8] * m[13] * m[6] - m[12] * m[5]
             * m[10] + m[4] * m[13] * m[10] + m[8] * m[5] * m[14] - m[4]
             * m[9] * m[14])
    / x;
    i[1] = (m[13] * m[10] * m[3] - m[9] * m[14] * m[3] - m[13] * m[2]
            * m[11] + m[1] * m[14] * m[11] + m[9] * m[2] * m[15] - m[1]
            * m[10] * m[15])
    / x;
    i[5] = (-m[12] * m[10] * m[3] + m[8] * m[14] * m[3] + m[12] * m[2]
            * m[11] - m[0] * m[14] * m[11] - m[8] * m[2] * m[15] + m[0]
            * m[10] * m[15])
    / x;
    i[9] = (m[12] * m[9] * m[3] - m[8] * m[13] * m[3] - m[12] * m[1]
            * m[11] + m[0] * m[13] * m[11] + m[8] * m[1] * m[15] - m[0]
            * m[9] * m[15])
    / x;
    i[13] = (-m[12] * m[9] * m[2] + m[8] * m[13] * m[2] + m[12] * m[1]
             * m[10] - m[0] * m[13] * m[10] - m[8] * m[1] * m[14] + m[0]
             * m[9] * m[14])
    / x;
    i[2] = (-m[13] * m[6] * m[3] + m[5] * m[14] * m[3] + m[13] * m[2]
            * m[7] - m[1] * m[14] * m[7] - m[5] * m[2] * m[15] + m[1]
            * m[6] * m[15])
    / x;
    i[6] = (m[12] * m[6] * m[3] - m[4] * m[14] * m[3] - m[12] * m[2] * m[7]
            + m[0] * m[14] * m[7] + m[4] * m[2] * m[15] - m[0] * m[6]
            * m[15])
    / x;
    i[10] = (-m[12] * m[5] * m[3] + m[4] * m[13] * m[3] + m[12] * m[1]
             * m[7] - m[0] * m[13] * m[7] - m[4] * m[1] * m[15] + m[0]
             * m[5] * m[15])
    / x;
    i[14] = (m[12] * m[5] * m[2] - m[4] * m[13] * m[2] - m[12] * m[1]
             * m[6] + m[0] * m[13] * m[6] + m[4] * m[1] * m[14] - m[0]
             * m[5] * m[14])
    / x;
    i[3] = (m[9] * m[6] * m[3] - m[5] * m[10] * m[3] - m[9] * m[2] * m[7]
            + m[1] * m[10] * m[7] + m[5] * m[2] * m[11] - m[1] * m[6]
            * m[11])
    / x;
    i[7] = (-m[8] * m[6] * m[3] + m[4] * m[10] * m[3] + m[8] * m[2] * m[7]
            - m[0] * m[10] * m[7] - m[4] * m[2] * m[11] + m[0] * m[6]
            * m[11])
    / x;
    i[11] = (m[8] * m[5] * m[3] - m[4] * m[9] * m[3] - m[8] * m[1] * m[7]
             + m[0] * m[9] * m[7] + m[4] * m[1] * m[11] - m[0] * m[5]
             * m[11])
    / x;
    i[15] = (-m[8] * m[5] * m[2] + m[4] * m[9] * m[2] + m[8] * m[1] * m[6]
             - m[0] * m[9] * m[6] - m[4] * m[1] * m[10] + m[0] * m[5]
             * m[10])
    / x;
    
    return true;
}
