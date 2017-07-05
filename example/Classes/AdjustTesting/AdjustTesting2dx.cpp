//
//  AdjustTesting2dx.cpp
//  Adjust Testing SDK
//
//  Created by Abdullah Obaied on 30/05/17.
//

#include "platform/android/jni/JniHelper.h"
#include "AdjustTesting2dx.h"
#include "AdjustTestingProxy2dx.h"
USING_NS_CC;

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
    
    jobject tmp = miInit.env->NewObject(clsTestLibrary, midInit, jBaseUrl, jCallbackProxy);
	this->testLibrary = cocos2d::JniHelper::getEnv()->NewGlobalRef(tmp);

    miInit.env->DeleteLocalRef(jBaseUrl);
    miInitCallback.env->DeleteLocalRef(jCallbackProxy);
}

void AdjustTesting2dx::initTestSession(std::string sdkPrefix) {
    if (this->testLibrary == NULL) {
        CCLOG("\n[*cocos*] >>>> JNI initTestSession() testLibrary is null");
        return;
    }

    cocos2d::JniMethodInfo miInitTestSession;

    if (!cocos2d::JniHelper::getMethodInfo(miInitTestSession, "com/adjust/testlibrary/TestLibrary", "initTestSession", "(Ljava/lang/String;)V")) {
        return;
    }
    
    jstring jSdkPrefix = miInitTestSession.env->NewStringUTF(sdkPrefix.c_str());
    
    miInitTestSession.env->CallVoidMethod(this->testLibrary, miInitTestSession.methodID, jSdkPrefix);
    
    miInitTestSession.env->DeleteLocalRef(jSdkPrefix);
}

void AdjustTesting2dx::addInfoToSend(std::string key, std::string value) {
    if (this->testLibrary == NULL) {
        CCLOG("\n[*cocos*] >>>> JNI addInfoToSend() testLibrary is null");
        return;
    }

    cocos2d::JniMethodInfo miAddInfoToSend;

    if (!cocos2d::JniHelper::getMethodInfo(miAddInfoToSend, "com/adjust/testlibrary/TestLibrary", "addInfoToSend", "(Ljava/lang/String;Ljava/lang/String;)V")) {
        return;
    }
    
    jstring jKey = miAddInfoToSend.env->NewStringUTF(key.c_str());
    jstring jValue = miAddInfoToSend.env->NewStringUTF(value.c_str());
    
    miAddInfoToSend.env->CallVoidMethod(this->testLibrary, miAddInfoToSend.methodID, jKey, jValue);

    miAddInfoToSend.env->DeleteLocalRef(jKey);
    miAddInfoToSend.env->DeleteLocalRef(jValue);
}

void AdjustTesting2dx::sendInfoToServer() {
    cocos2d::JniMethodInfo miSendInfoToServer;

    if (!cocos2d::JniHelper::getMethodInfo(miSendInfoToServer, "com/adjust/testlibrary/TestLibrary", "sendInfoToServer", "()V")) {
        return;
    }

    miSendInfoToServer.env->CallVoidMethod(this->testLibrary, miSendInfoToServer.methodID);
}

void AdjustTesting2dx::setTests(std::string selectedTests) {
    if (this->testLibrary == NULL) {
        CCLOG("\n[*cocos*] >>>> JNI setTests(): testLibrary is null");
        return;
    }

    cocos2d::JniMethodInfo miSetTests;

    if (!cocos2d::JniHelper::getMethodInfo(miSetTests, "com/adjust/testlibrary/TestLibrary", "setTests", "(Ljava/lang/String;)V")) {
        return;
    }
    
    jstring jSelectedTests = miSetTests.env->NewStringUTF(selectedTests.c_str());
    
    miSetTests.env->CallVoidMethod(this->testLibrary, miSetTests.methodID, jSelectedTests);

    miSetTests.env->DeleteLocalRef(jSelectedTests);
}

