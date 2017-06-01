LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../../Classes/AppDelegate.cpp \
                   ../../../Classes/HelloWorldScene.cpp \
                   ../../../Classes/Adjust/AdjustConfig2dx.cpp \
                   ../../../Classes/Adjust/AdjustAttribution2dx.cpp \
                   ../../../Classes/Adjust/AdjustProxy2dx.cpp \
                   ../../../Classes/Adjust/AdjustEvent2dx.cpp \
                   ../../../Classes/Adjust/Adjust2dx.cpp \
                   ../../../Classes/Adjust/AdjustEventFailure2dx.cpp \
                   ../../../Classes/Adjust/AdjustEventSuccess2dx.cpp \
                   ../../../Classes/Adjust/AdjustSessionFailure2dx.cpp \
                   ../../../Classes/Adjust/AdjustSessionSuccess2dx.cpp \
                   ../../../Classes/AdjustTesting/AdjustTesting2dx.cpp \
                   ../../../Classes/AdjustTesting/AdjustTestingProxy2dx.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
