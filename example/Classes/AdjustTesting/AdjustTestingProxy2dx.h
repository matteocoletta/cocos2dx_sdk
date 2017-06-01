//
//  AdjustTestingProxy2dx.h
//  Adjust Testing SDK
//
//  Created by Abdullah Obaied on 30/05/17.
//
#ifndef ADJUSTTESTING_ADJUSTTESTINGPROXY2DX_H_
#define ADJUSTTESTING_ADJUSTTESTINGPROXY2DX_H_

#include <jni.h>
#include <iostream>

extern "C" {
    static void (*commandCallbackMethod)(std::string className, std::string methodName, std::string jsonParams);

    JNIEXPORT void JNICALL Java_com_adjust_testlibrary_AdjustTesting2dxCommandCallback_commandExecuted(JNIEnv *, jobject, jstring, jstring, jstring);

    void setCommandCallbackMethod(void (*callbackMethod)(std::string className, std::string methodName, std::string jsonParams));
}
#endif /* ADJUSTTESTING_ADJUSTTESTINGPROXY2DX_H_ */
