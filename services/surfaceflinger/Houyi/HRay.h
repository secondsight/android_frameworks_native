//
//  HRay.h
//  houyi
//
//  Created by Lance Mao on 9/10/11.
//  Copyright (c) 2011 Dong wen. All rights reserved.
//

#ifndef houyi_HRay_h
#define houyi_HRay_h

#include "HVector.h"

class HRay
{
public:
    HVector p;
    HVector d;
    
    HRay(HVector ap, HVector ad);
};

#endif
