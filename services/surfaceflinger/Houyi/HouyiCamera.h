//
//  HouyiCamera.h
//  Houyi
//
//  Created by Lance Mao on 16/2/13.
//  Copyright (c) 2013 Lance Mao. All rights reserved.
//

#ifndef __Houyi__HouyiCamera__
#define __Houyi__HouyiCamera__

#include "HRay.h"

namespace Houyi
{
    class Camera
    {
    protected:
        /** Position of the camera's eye point. */
        HVector mPosition;
        /** Position of the "look-at" direction point. */
        HVector mLookAt;
        /** Direction of the up vector. */
        HVector mUp;

        HVector mRight;
        HVector mForward;

        float mViewMatrix[16];
        float mViewInvMatrix[16];
        float mProjectionMatrix[16];
        float mInvProjectionMatrix[16];
        float mNormalMatrix[9];

        // project * view matrix
        float mViewProjectionMatrix[16];

        float mBankAngle;
        float mFocus;
        float mNearRange;
        float mFarRange;
        float mRoll;
        float mFOV;
        float mAspectRatio;
        bool mIsAspectRatioDirty;

    public:
        float** mKFTargetPos;
        int mKFTargetPosLength;
        float** mKFFOV;
        int mKFFOVLength;
        float** mKFRoll;
        int mKFRollLength;

        void buildViewMatrix();
        void buildInvViewMatrix();

    public:
        virtual ~Camera();
        Camera(float aspectRatio = 1.0f);
        Camera(HVector position, HVector lookat, float roll, float aspectRatio = 0);
        void init();

        HVector getLookAt();
        void setLookAt(float x, float y, float z);
        HVector getPosition();
        void setPosition(float x, float y, float z);
        HVector getUp();
        void setUp(float x, float y, float z);
        float getBankAngle();
        void setBankAngle(float angle);
        float getFocus();
        void setFocus(float focus);
        float getNearRange();
        void setNearRange(float nearrange);
        float getFarRange();
        void setFarRange(float farrange);
        float getFOV();
        void setFOV(float fov);
        float getRoll();
        void setRoll(float roll);
        float getPeekSpeed();
        void setPeekSpeed(float peekSpeed);
        float getAspectRatio();
        void setAspectRatio(float aspectRatio);
        bool isAspectRatioDirty();
        void setAspectRatioDirty(bool isDirty);

        float* getViewMatrix();
        float* getProjectionMatrix();
        float* getViewProjectionMatrix();
        float* getNormalMatrix();

        virtual void lookAt();
        virtual void lookAt(HVector aPosition, HVector aLookAt, float aRoll);

        virtual void update(int keyFrame);
        virtual void update();

        virtual void project();
        virtual void perspective();
        virtual void perspective(float fov, float aspectRatio, float near, float far);

        virtual HVector clipToWorld(float screenX, float screenY);
        virtual HRay castRay(float ndcX, float ndcY);
        virtual HRay castRay(float screenX, float screenY, float viewportWidth, float viewportHeight);

        //virtual void setTransform(HTransform trans);

        virtual void setKFTargetPos(int index, float x, float y, float z);
        virtual void setupKeyFrame();
    };
}

#endif /* defined(__Houyi__HouyiCamera__) */
