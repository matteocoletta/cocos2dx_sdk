//
//  AdjustTesting2dx.cpp
//  Adjust Testing SDK
//
//  Created by Abdullah Obaied on 30/05/17.
//

#include "platform/android/jni/JniHelper.h"
#include "AdjustTesting2dx.h"
#include "AdjustTestingProxy2dx.h"

void AdjustTesting2dx::initTesting(std::string baseUrl, void(*commandCallback)(std::string className, std::string methodName, std::string jsonParams)) {
    //Callback
    //======================
    setCommandCallbackMethod(commandCallback);

    cocos2d::JniMethodInfo miInitCallback;
    if (!cocos2d::JniHelper::getMethodInfo(miInitCallback, "com/adjust/testlibrary/AdjustTesting2dxCommandCallback", "<init>", "()V")) {
        return;
    }

    jclass clsAdjustTesting2dxCommandCallback = miInitCallback.env->FindClass("com/adjust/testlibrary/AdjustTesting2dxCommandCallback");
    jmethodID midInitCallback = miInitCallback.env->GetMethodID(clsAdjustTesting2dxCommandCallback, "<init>", "()V");
    jobject jCallbackProxy = miInitCallback.env->NewObject(clsAdjustTesting2dxCommandCallback, midInitCallback);

    //TestLibrary
    //======================
    cocos2d::JniMethodInfo miInit;
    if (!cocos2d::JniHelper::getMethodInfo(miInit, "com/adjust/testlibrary/TestLibrary", "<init>", "(Ljava/lang/String;Lcom/adjust/testlibrary/ICommandJsonListener;)V")) {
        return;
    }

    //jBaseUrl
    //======================
    jstring jBaseUrl = miInit.env->NewStringUTF(baseUrl.c_str());

    jclass clsTestLibrary = miInit.env->FindClass("com/adjust/testlibrary/TestLibrary");
    jmethodID midInit = miInit.env->GetMethodID(clsTestLibrary, "<init>", "(Ljava/lang/String;Lcom/adjust/testlibrary/ICommandJsonListener;)V");
    
    testLibrary = miInit.env->NewObject(clsTestLibrary, midInit, jBaseUrl, jCallbackProxy);

    miInit.env->DeleteLocalRef(jBaseUrl);
    miInitCallback.env->DeleteLocalRef(jCallbackProxy);
}

void AdjustTesting2dx::initTestSession(std::string sdkPrefix) {
    cocos2d::JniMethodInfo miInitTestSession;

    if (!cocos2d::JniHelper::getMethodInfo(miInitTestSession, "com/adjust/testlibrary/TestLibrary", "initTestSession", "(Ljava/lang/String;)V")) {
        return;
    }
    
    jstring jSdkPrefix = miInitTestSession.env->NewStringUTF(sdkPrefix.c_str());
    
    miInitTestSession.env->CallVoidMethod(testLibrary, miInitTestSession.methodID, jSdkPrefix);
    
    miInitTestSession.env->DeleteLocalRef(jSdkPrefix);
}
