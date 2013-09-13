//
//  HVector.cpp
//  houyi
//
//  Created by Lance Mao on 12/08/2011.
//  Copyright 2011 Dong wen. All rights reserved.
//

#include "HVector.h"
#include "HMatrix.h"

HVector HVector::Zero;
HVector HVector::BasisX (1, 0, 0);
HVector HVector::BasisY (0, 1, 0);
HVector HVector::BasisZ (0, 0, 1);

HVector HVector::operator*(const HMatrix& m) const
{
    float w = m[3] + m[7] + m[11] + m[15];
    if (w == 0){
        return *this;
    }
    
    return HVector((m[0] * (*this).x + m[4] * (*this).y + m[8] * (*this).z + m[12]) / w,
                   (m[1] * (*this).x + m[5] * (*this).y + m[9] * (*this).z + m[13]) / w,
                   (m[2] * (*this).x + m[6] * (*this).y + m[10]* (*this).z + m[14]) / w);
}

HVector& HVector::operator*=(const HMatrix& matrix)
{
    HVector res = (*this) * matrix;
    return (*this) = res;
}
