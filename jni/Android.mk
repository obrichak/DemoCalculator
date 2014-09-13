LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := DemoCalculator

LOCAL_ARM_MODE := arm
LOCAL_DEFAULT_CPP_EXTENSION := cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)

LOCAL_SRC_FILES := DemoCalculator.cpp

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
