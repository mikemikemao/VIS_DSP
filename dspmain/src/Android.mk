LOCAL_PATH:= $(call my-dir)
HOME_DIR:= $(LOCAL_PATH)/..

include $(CLEAR_VARS)
LOCAL_SRC_FILES := 	\
	dspmain.cpp \

LOCAL_SHARED_LIBRARIES:= \
    liblog \
    libui \
	libgui \
    libbinder \
    libutils \
    libcutils \
    libcamera_client \
    libhardware \
    libandroid \
    libandroidfw \
	libstagefright \
	libstagefright_foundation \
	libmedia \
	libEGL \
	libGLESv2 \


LOCAL_C_INCLUDES := \
	Audio \
	Tools \
	frameworks/av/include \
	frameworks/av/media/libstagefright \
	frameworks/av/media/libstagefright/include \
	frameworks/av/media/libstagefright/foundation/include \
	frameworks/av/media/libstagefright/foundation/include/media/stagefright/foundation \
	frameworks/av/media/libmedia/include \
	frameworks/av/media/libmedia/include/media\
	frameworks/av/media/libaudioclient/include/media \
	frameworks/av/media/libmediametrics \
	frameworks/native/include/media/openmax \
	frameworks/base/native/include \
	frameworks/base/core/jni/android \
	frameworks/base/include/ui \
	frameworks/native/include \
	hardware/rockchip/librkvpu \
	system/core/include \
	bionic \


# -g  -rdynamic  -funwind-tables
#用于在SDK\out\target\product\rk3288\obj\SHARED_LIBRARIES\libdsp_intermediates\PACKED
#目录下生成待调试信息的动态库
LOCAL_CFLAGS += -Wall $(SAL_FLAGS) $(SVN_FLAGS) -DCONFIG_MULTI_CAMERAS_SUPPORT -DCONFIG_PLATFORM_ROCKCHIP 
LOCAL_32_BIT_ONLY := true
LOCAL_CLANG := true
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_MODULE_TAGS := optional

#指定生成的动态库名称
LOCAL_MODULE:= dspserver

#指定动态库生成路径
#LOCAL_MODULE_PATH := ~/nfs/android8

TARGET_PRELINK_MODULE := false

include $(BUILD_EXECUTABLE)

$(info $(LOCAL_C_INCLUDES))
$(info $(LOCAL_LDFLAGS))
$(info $(SAL_MODULE_NAME))
$(info $(LOCAL_CFLAGS))
$(info $(LOCAL_PATH))
