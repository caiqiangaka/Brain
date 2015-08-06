LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/ipc
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog

LOCAL_MODULE      := NDK_ipc
LOCAL_SRC_FILES   := \
	../ipc/cJSON.c \
	../ipc/communicate_lib.c \
	../ipc/udp_communicate_driver.c \
	../ipc/unix_communicate_driver.c
	

include $(BUILD_SHARED_LIBRARY)

	