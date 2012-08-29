LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := liblua
include source_liblua.mak
LOCAL_CFLAGS += -w -fsigned-char
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := libgameswf
include source_gameswf.mak
LOCAL_CFLAGS += -w -fsigned-char
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := libirrlicht
include source_irlicht.mak
LOCAL_CFLAGS += -w -fsigned-char
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := libirrlichtex
include source_irlichtex.mak
LOCAL_CFLAGS += -w -fsigned-char
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := libgsgameplay
include source_gameplay.mak
LOCAL_CFLAGS += -w -fsigned-char
LOCAL_STATIC_LIBRARIES := liblua libgameswf libirrlicht libirrlichtex
LOCAL_LDLIBS := -lGLESv2 -ldl -llog -lc -lgcc
include $(BUILD_SHARED_LIBRARY)