
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := libfreetype
include source_libfreetype.mak
LOCAL_CFLAGS += -w -fsigned-char
include $(BUILD_STATIC_LIBRARY)