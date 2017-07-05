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
    jobject testLibrary;

    void initTesting(std::string baseUrl, void(*commandCallback)(std::string className, std::string methodName, std::string jsonParams));

public:
    AdjustTesting2dx() {}
    ~AdjustTesting2dx() {
	    cocos2d::JniHelper::getEnv()->DeleteGlobalRef(this->testLibrary);
        this->testLibrary = NULL;
    }

    AdjustTesting2dx(std::string baseUrl, void(*commandCallback)(std::string className, std::string methodName, std::string jsonParams)) {
        initTesting(baseUrl, commandCallback);
    }

    void initTestSession(std::string sdkPrefix);
    void addInfoToSend(std::string key, std::string value);
    void sendInfoToServer();
    void setTests(std::string selectedTests);
};
#endif

#endif /* ADJUSTTESTING_ADJUSTTESTING2DX_H_ */
