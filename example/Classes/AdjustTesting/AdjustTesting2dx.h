//
//  AdjustTesting2dx.h
//  Adjust Testing SDK
//
//  Created by Abdullah Obaied on 30/05/17.
//

#ifndef ADJUSTTESTING_ADJUSTTESTING2DX_H_
#define ADJUSTTESTING_ADJUSTTESTING2DX_H_

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <iostream>

class AdjustTesting2dx {
private:
    jobject kotlinLibrary;
    jobject staticKotlinLibrary;

    void initKotlin();
    void initStaticKotlin();

public:
    AdjustTesting2dx() {
        initKotlin();
        initStaticKotlin();
    }

    ~AdjustTesting2dx() {
	    cocos2d::JniHelper::getEnv()->DeleteGlobalRef(this->kotlinLibrary);
	    cocos2d::JniHelper::getEnv()->DeleteGlobalRef(this->staticKotlinLibrary);
        this->kotlinLibrary = NULL;
        this->staticKotlinLibrary = NULL;
    }

    void getFoo();
    void getStaticFoo();
};
#endif

#endif /* ADJUSTTESTING_ADJUSTTESTING2DX_H_ */
