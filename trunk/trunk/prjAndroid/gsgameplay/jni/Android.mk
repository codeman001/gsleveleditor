LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := libfreetype
include source_libfreetype.mak
LOCAL_CFLAGS += -w -fsigned-char
include $(BUILD_STATIC_LIBRARY)

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
LOCAL_MODULE    := libgsgameplay
include source_gameplay.mak
LOCAL_CFLAGS += -w -fsigned-char
include $(BUILD_SHARED_LIBRARY)