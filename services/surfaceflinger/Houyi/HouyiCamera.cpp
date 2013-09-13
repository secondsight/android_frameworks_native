//
//  HouyiCamera.cpp
//  Houyi
//
//  Created by Lance Mao on 16/2/13.
//  Copyright (c) 2013 Lance Mao. All rights reserved.
//

#include <math.h>

#include "HouyiCamera.h"
#include "HMath.h"
#include "HMatrix.h"

namespace Houyi
{
    Camera::~Camera()
    {
        /*for (int i = 0;i < mKFPosLength;i++){
         delete [] mKFPos[i];
         }
         delete [] mKFPos;*/

        for (int i = 0; i < mKFTargetPosLength; i++)
        {
            delete[] mKFTargetPos[i];
        }
        delete[] mKFTargetPos;

        for (int i = 0; i < mKFFOVLength; i++)
        {
            delete[] mKFFOV[i];
        }
        delete[] mKFFOV;

        for (int i = 0; i < mKFRollLength; i++)
        {
            delete[] mKFRoll[i];
        }
        delete[] mKFRoll;
    }

    Camera::Camera(float aspectRatio) :
            mPosition(0, 0, 1), mLookAt(0, 0, -1), mUp(0, 1, 0), mRight(1, 0, 0), mForward(0, 0, -1), mAspectRatio(
                    aspectRatio)
    {
        init();
        perspective();
    }

    Camera::Camera(HVector position, HVector lookat, float roll, float aspectRatio) :
            mPosition(position), mLookAt(lookat), mRoll(roll), mAspectRatio(aspectRatio)
    {
        init();
        lookAt(position, lookat, roll);
        perspective();
    }

    void Camera::init()
    {
        mNearRange = 1;
        mFarRange = 1000;
        mFOV = 30;
        mRoll = 0;
        if (mAspectRatio == 0)
        {
            mAspectRatio = 1.0f;
            mIsAspectRatioDirty = true;
        }
//    mMaxKFCount = 1;

//    mKFPos = NULL;
        mKFTargetPos = NULL;
        mKFFOV = NULL;
        mKFRoll = NULL;
//    mKFPosLength = 0;
        mKFTargetPosLength = 0;
        mKFFOVLength = 0;
        mKFRollLength = 0;
    }

    HVector Camera::getLookAt()
    {
        return mLookAt;
    }

    void Camera::setLookAt(float x, float y, float z)
    {
        mLookAt.x = x;
        mLookAt.y = y;
        mLookAt.z = z;
    }

    HVector Camera::getPosition()
    {
        return mPosition;
    }

    void Camera::setPosition(float x, float y, float z)
    {
        mPosition.x = x;
        mPosition.y = y;
        mPosition.z = z;
    }

    HVector Camera::getUp()
    {
        return mUp;
    }

    void Camera::setUp(float x, float y, float z)
    {
        mUp.x = x;
        mUp.y = y;
        mUp.z = z;
    }

    float Camera::getBankAngle()
    {
        return mBankAngle;
    }

    void Camera::setBankAngle(float angle)
    {
        mBankAngle = angle;
    }

    float Camera::getFocus()
    {
        return mFocus;
    }

    void Camera::setFocus(float focus)
    {
        mFocus = focus;
    }

    float Camera::getNearRange()
    {
        return mNearRange;
    }

    void Camera::setNearRange(float nearrange)
    {
        mNearRange = nearrange;
    }

    float Camera::getFarRange()
    {
        return mFarRange;
    }

    void Camera::setFarRange(float farrange)
    {
        mFarRange = farrange;
    }

    float Camera::getFOV()
    {
        return mFOV;
    }

    void Camera::setFOV(float fov)
    {
        mFOV = fov;
    }

    float Camera::getRoll()
    {
        return mRoll;
    }

    void Camera::setRoll(float roll)
    {
        mRoll = roll;
    }

    float* Camera::getViewMatrix()
    {
        return mViewMatrix;
    }

    float Camera::getAspectRatio()
    {
        return mAspectRatio;
    }

    void Camera::setAspectRatio(float aspectRatio)
    {
        mAspectRatio = aspectRatio;
        mIsAspectRatioDirty = false;
        perspective();
    }

    bool Camera::isAspectRatioDirty()
    {
        return mIsAspectRatioDirty;
    }

    void Camera::setAspectRatioDirty(bool isDirty)
    {
        mIsAspectRatioDirty = isDirty;
    }

    float* Camera::getProjectionMatrix()
    {
        return mProjectionMatrix;
    }

    float* Camera::getViewProjectionMatrix()
    {
        return mViewProjectionMatrix;
    }

    float* Camera::getNormalMatrix()
    {
        return mNormalMatrix;
    }

    void Camera::lookAt()
    {
        lookAt(mPosition, mLookAt, mRoll);
    }

    void Camera::lookAt(HVector aPosition, HVector aLookAt, float aRoll)
    {
        mPosition = aPosition;
        mLookAt = aLookAt;

        mUp.x = 0;
        mUp.y = 1;
        mUp.z = 0;

        mForward = aLookAt.substract(aPosition);
        mForward.normalize();

        // calculating up vector using the following steps:
        // 1. Calculate angle of mForward & x-z plane: theta1
        // 2. Calculate angle of mForward & -z axis: theta2
        // 3. Rotate (0, 1, 0) along x axis by theta1
        // 4. Rotate then along y axis by theta2 or 180-theta2
        // 5. Rotate then along mForward by aRoll
        double theta1 = asin(mForward.y);

        double theta2;
        if (mForward.z == 0)
        {
            theta2 = 0;
        }
        else
        {
            theta2 = atan(mForward.x / mForward.z);
            if (mForward.z > 0)
            {
                theta2 = PI / 2 - theta2;
            }
        }

        mUp.y = (float) cos(theta1);
        mUp.z = (float) sin(theta1);

        mUp.z = (float) (mUp.z * cos(theta2) - mUp.x * sin(theta2));
        mUp.x = (float) (mUp.x * cos(theta2) + mUp.z * sin(theta2));

        aRoll = HMath::degree2radian(aRoll);
        mUp.rotate(mForward, aRoll);
        // up.iX = up.iX * Cos(aRoll) - up.iY * Sin(aRoll);
        // up.iY = up.iY * Cos(aRoll) + up.iZ * Sin(aRoll);

        mUp.normalize();

        mRight = mForward.crossProduct(mUp);
        mRight.normalize();

        mUp = mRight.crossProduct(mForward);
        mUp.normalize();

        buildViewMatrix();
    }

    void Camera::update(int keyFrame)
    {
//    mCurFrame = keyFrame % mMaxKFCount;
//    
//    bool lookat = false;
//    if (mKFPos != NULL && mKFPosLength > 1){
//        if (mCurFrame >= 0 && mCurFrame < mKFPosLength){
//            mPosition.x = mKFPos[mCurFrame][0];
//            mPosition.y = mKFPos[mCurFrame][1];
//            mPosition.z = mKFPos[mCurFrame][2];
//        }
//        
//        lookat = true;
//    }
//    
//    if (mKFTargetPos != NULL && mKFTargetPosLength > 1){
//        if (mCurFrame >= 0 && mCurFrame < mKFTargetPosLength){
//            mLookAt.x = mKFTargetPos[mCurFrame][0];
//            mLookAt.y = mKFTargetPos[mCurFrame][1];
//            mLookAt.z = mKFTargetPos[mCurFrame][2];
//        }
//        
//        lookat = true;
//    }
//    
//    if (lookat){
//        lookAt();
//    }else{
//        update();
//    }
    }

    void Camera::update()
    {
        mForward = mLookAt.substract(mPosition);
        mForward.normalize();

        mRight = mForward.crossProduct(mUp);
        mRight.normalize();

        mUp = mRight.crossProduct(mForward);
        mUp.normalize();

        buildViewMatrix();
    }

    void Camera::project()
    {
        perspective();
    }

    void Camera::perspective()
    {
        perspective(mFOV, mAspectRatio, mNearRange, mFarRange);
    }

    void Camera::perspective(float fov, float aspectRatio, float near, float far)
    {
        mNearRange = near;
        mFarRange = far;
        mAspectRatio = aspectRatio;

        float f = (float) (1 / tan(HMath::degree2radian(fov / 2)));
        float epsilon = 0.0000048;
        epsilon = 0;
        mProjectionMatrix[0 * 4 + 0] = f / aspectRatio;
        mProjectionMatrix[1 * 4 + 0] = 0;
        mProjectionMatrix[2 * 4 + 0] = 0;
        mProjectionMatrix[3 * 4 + 0] = 0;

        mProjectionMatrix[0 * 4 + 1] = 0;
        mProjectionMatrix[1 * 4 + 1] = f;
        mProjectionMatrix[2 * 4 + 1] = 0;
        mProjectionMatrix[3 * 4 + 1] = 0;

        mProjectionMatrix[0 * 4 + 2] = 0;
        mProjectionMatrix[1 * 4 + 2] = 0;
        mProjectionMatrix[2 * 4 + 2] = (near + far) / (near - far) - epsilon;
        mProjectionMatrix[3 * 4 + 2] = 2 * near * far / (near - far);

        mProjectionMatrix[0 * 4 + 3] = 0;
        mProjectionMatrix[1 * 4 + 3] = 0;
        mProjectionMatrix[2 * 4 + 3] = -1;
        mProjectionMatrix[3 * 4 + 3] = 0;

        HMatrix::inverse(mInvProjectionMatrix, mProjectionMatrix);
    }

    void Camera::buildViewMatrix()
    {
        mViewMatrix[0 * 4 + 0] = mNormalMatrix[0 * 3 + 0] = mRight.x;
        mViewMatrix[1 * 4 + 0] = mNormalMatrix[1 * 3 + 0] = mRight.y;
        mViewMatrix[2 * 4 + 0] = mNormalMatrix[2 * 3 + 0] = mRight.z;
        mViewMatrix[3 * 4 + 0] = -mPosition.x * mRight.x - mPosition.y * mRight.y - mPosition.z * mRight.z;

        mViewMatrix[0 * 4 + 1] = mNormalMatrix[0 * 3 + 1] = mUp.x;
        mViewMatrix[1 * 4 + 1] = mNormalMatrix[1 * 3 + 1] = mUp.y;
        mViewMatrix[2 * 4 + 1] = mNormalMatrix[2 * 3 + 1] = mUp.z;
        mViewMatrix[3 * 4 + 1] = -mPosition.x * mUp.x - mPosition.y * mUp.y - mPosition.z * mUp.z;

        mViewMatrix[0 * 4 + 2] = mNormalMatrix[0 * 3 + 2] = -mForward.x;
        mViewMatrix[1 * 4 + 2] = mNormalMatrix[1 * 3 + 2] = -mForward.y;
        mViewMatrix[2 * 4 + 2] = mNormalMatrix[2 * 3 + 2] = -mForward.z;
        mViewMatrix[3 * 4 + 2] = mPosition.x * mForward.x + mPosition.y * mForward.y + mPosition.z * mForward.z;

        mViewMatrix[0 * 4 + 3] = 0;
        mViewMatrix[1 * 4 + 3] = 0;
        mViewMatrix[2 * 4 + 3] = 0;
        mViewMatrix[3 * 4 + 3] = 1;
    }

    void Camera::buildInvViewMatrix()
    {
        mForward = mLookAt.substract(mPosition);
        mForward.normalize();
        mRight = mForward.crossProduct(mUp);
        mRight.normalize();
        mUp = mRight.crossProduct(mForward);
        mUp.normalize();
        mViewInvMatrix[0 * 4 + 0] = mRight.x;
        mViewInvMatrix[1 * 4 + 0] = mUp.x;
        mViewInvMatrix[2 * 4 + 0] = -mForward.x;
        mViewInvMatrix[3 * 4 + 0] = mPosition.x;
        mViewInvMatrix[0 * 4 + 1] = mRight.y;
        mViewInvMatrix[1 * 4 + 1] = mUp.y;
        mViewInvMatrix[2 * 4 + 1] = -mForward.y;
        mViewInvMatrix[3 * 4 + 1] = mPosition.y;
        mViewInvMatrix[0 * 4 + 2] = mRight.z;
        mViewInvMatrix[1 * 4 + 2] = mUp.z;
        mViewInvMatrix[2 * 4 + 2] = -mForward.z;
        mViewInvMatrix[3 * 4 + 2] = mPosition.z;
        mViewInvMatrix[0 * 4 + 3] = 0;
        mViewInvMatrix[1 * 4 + 3] = 0;
        mViewInvMatrix[2 * 4 + 3] = 0;
        mViewInvMatrix[3 * 4 + 3] = 1;
    }

    HVector Camera::clipToWorld(float screenX, float screenY)
    {
        // 1. transform screen coordinates into eye coordinates
        HVector viewCoord = HMatrix::multiply(mInvProjectionMatrix, screenX, screenY, 0);

        // 2. transform eye coordinates into world coordinates using inverse of view matrix
        // note the view matrix is an orthogonal matrix, so we can get the inverse of
        // this sub matrix immediately because its inverse is its transpose.
        // Then we multiply translation to get the final matrix
        buildInvViewMatrix();
        HVector worldCoord = HMatrix::multiply(mViewInvMatrix, viewCoord);
        return worldCoord;
    }

    HRay Camera::castRay(float ndcX, float ndcY)
    {
        HVector p = clipToWorld(ndcX, ndcY);
        HVector d = p - mPosition;
        d.normalize();
        return HRay(mPosition, d);
    }

    HRay Camera::castRay(float screenX, float screenY, float viewportWidth, float viewportHeight)
    {
        float ndcX = screenX * 2 / viewportWidth - 1;
        float ndcY = 1 - screenY * 2 / viewportHeight;
        return castRay(ndcX, ndcY);
    }


    void Camera::setKFTargetPos(int index, float x, float y, float z)
    {
        mKFTargetPos[index][0] = x;
        mKFTargetPos[index][1] = y;
        mKFTargetPos[index][2] = z;
    }

    void Camera::setupKeyFrame()
    {
    }
}
