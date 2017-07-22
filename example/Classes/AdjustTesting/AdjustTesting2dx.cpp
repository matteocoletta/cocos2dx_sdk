//
//  AdjustTesting2dx.cpp
//  Adjust Testing SDK
//
//  Created by Abdullah Obaied on 30/05/17.
//

#include "platform/android/jni/JniHelper.h"
#include "AdjustTesting2dx.h"
USING_NS_CC;

void AdjustTesting2dx::initKotlin() {
    //TestLibrary
    //======================
    cocos2d::JniMethodInfo miInit;
    if (!cocos2d::JniHelper::getMethodInfo(miInit, "com/obaied/testlibrary/MyTestClass", "<init>", "()V")) {
        return;
    }

    jclass clsTestLibrary = miInit.env->FindClass("com/obaied/testlibrary/MyTestClass");
    jmethodID midInit = miInit.env->GetMethodID(clsTestLibrary, "<init>", "()V");

    jobject tmp = miInit.env->NewObject(clsTestLibrary, midInit);
    this->kotlinLibrary = cocos2d::JniHelper::getEnv()->NewGlobalRef(tmp);
}

void AdjustTesting2dx::initStaticKotlin() {
    //TestLibrary
    //======================
    cocos2d::JniMethodInfo miInit;
    if (!cocos2d::JniHelper::getMethodInfo(miInit, "com/obaied/testlibrary/MyTestClass$Companion", "<init>", "()V")) {
        return;
    }

    jclass clsTestLibrary = miInit.env->FindClass("com/obaied/testlibrary/MyTestClass$Companion");
    jmethodID midInit = miInit.env->GetMethodID(clsTestLibrary, "<init>", "()V");

    jobject tmp = miInit.env->NewObject(clsTestLibrary, midInit);
    this->staticKotlinLibrary = cocos2d::JniHelper::getEnv()->NewGlobalRef(tmp);
}

void AdjustTesting2dx::getFoo() {
    if (this->kotlinLibrary == NULL) {
        CCLOG("\n[*cocos*] >>>> JNI getFoo() kotlinLibrary is null");
        return;
    }

    CCLOG("\n[*cocos*] >>>> JNI getFoo() 1 >>>>");
    cocos2d::JniMethodInfo miGetFoo;
    if (!cocos2d::JniHelper::getMethodInfo(miGetFoo, "com/obaied/testlibrary/MyTestClass", "getFoo", "()V")) {
        return;
    }
    CCLOG("\n[*cocos*] >>>> JNI getFoo() 2 >>>>");

    miGetFoo.env->CallVoidMethod(this->kotlinLibrary, miGetFoo.methodID);
}

void AdjustTesting2dx::getStaticFoo() {
    if (this->staticKotlinLibrary == NULL) {
        CCLOG("\n[*cocos*] >>>> JNI getStaticFoo() staticKotlinLibrary is null");
        return;
    }

    CCLOG("\n[*cocos*] >>>> JNI getStaticFoo() 1 >>>>");
    cocos2d::JniMethodInfo miGetStaticFoo;
    if (!cocos2d::JniHelper::getMethodInfo(miGetStaticFoo, "com/obaied/testlibrary/MyTestClass$Companion", "getStaticFoo", "()V")) {
        return;
    }
    CCLOG("\n[*cocos*] >>>> JNI getStaticFoo() 2 >>>>");

    miGetStaticFoo.env->CallVoidMethod(this->staticKotlinLibrary, miGetStaticFoo.methodID);
}
