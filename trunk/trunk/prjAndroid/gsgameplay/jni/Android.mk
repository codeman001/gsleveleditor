
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := libfreetype
include source_libfreetype.mak
LOCAL_CFLAGS += -w -fsigned-char
include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    := libcurl
include source_liburl.mak
LOCAL_CFLAGS += -w -fsigned-char
include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := libapptoado
include source_toado.mak
LOCAL_CFLAGS += -w -fsigned-char
LOCAL_STATIC_LIBRARIES := libfreetype libcurl
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lc -lgcc
include $(BUILD_SHARED_LIBRARY)