//
//  AdjustTestingProxy2dx.cpp
//  Adjust Testing SDK
//
//  Created by Abdullah Obaied on 30/05/17.
//

#include "AdjustTestingProxy2dx.h"

JNIEXPORT void JNICALL Java_com_adjust_testlibrary_AdjustTesting2dxCommandCallback_commandExecuted(JNIEnv *env, jobject obj, jstring jClassName, jstring jMethodName, jstring jJsonParams) {
    if (NULL == commandCallbackMethod) {
        return;
    }

    std::string className;
    std::string methodName;
    std::string jsonParams;

    if (NULL != jClassName) {
        const char *classNameCStr = env->GetStringUTFChars(jClassName, NULL);
        className = std::string(classNameCStr);
        env->ReleaseStringUTFChars(jClassName, classNameCStr);
        env->DeleteLocalRef(jClassName);
    } else {
        className = "";
    }

    if (NULL != jMethodName) {
        const char *methodNameCStr = env->GetStringUTFChars(jMethodName, NULL);
        methodName = std::string(methodNameCStr);
        env->ReleaseStringUTFChars(jMethodName, methodNameCStr);
        env->DeleteLocalRef(jMethodName);
    } else {
        methodName = "";
    }

    if (NULL != jJsonParams) {
        const char *jsonParamsCStr = env->GetStringUTFChars(jJsonParams, NULL);
        jsonParams = std::string(jsonParamsCStr);
        env->ReleaseStringUTFChars(jJsonParams, jsonParamsCStr);
        env->DeleteLocalRef(jJsonParams);
    } else {
        jsonParams = "";
    }

    commandCallbackMethod(className, methodName, jsonParams);
}

void setCommandCallbackMethod(void (*callbackMethod)(std::string className, std::string methodName, std::string jsonParams)) {
    if (NULL == commandCallbackMethod) {
    	commandCallbackMethod = callbackMethod;
    }
}
