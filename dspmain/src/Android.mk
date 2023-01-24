DSP_PATH := $(call my-dir)
LOCAL_PATH := $(DSP_PATH)

COMPILE_USER := $(BUILD_USERNAME)
$(info $(COMPILE_USER))

include $(LOCAL_PATH)/../DSP_Common.mk

include $(CLEAR_VARS)

#get all sub-dir
DSP_SRC_PATH := $(call all-c-files-under)
DSP_SRC_PATH += $(call all-cpp-files-under)

LOCAL_SRC_FILES         := $(DSP_SRC_PATH)

LOCAL_SHARED_LIBRARIES  := $(DSP_ANDROID_BASE_SHARED_LIBS) $(DSP_SERVER_NDK_SHARED_LIBS) $(DSP_GRALLOC_LIB4) hardware.hik.dspservice@2.0 libaudiohal
LOCAL_HEADER_LIBRARIES  := libaudiohal_headers
LOCAL_C_INCLUDES        := $(DSP_INC_MODULES)
LOCAL_CFLAGS            := -Werror -Wall -g

BUILD_DATE := $(shell date +"%Y-%m-%d")
BUILD_TIME := $(shell date +"%H:%M:%S")


ifeq ($(strip $(DSP_PLAT_BITS)), 64)
LOCAL_CFLAGS            +=  -DBANDWIDTH_64BIT
LOCAL_CFLAGS            +=  -D_ANDROID64
endif

LOCAL_CFLAGS            += $(DSP_CFLAGS)


LOCAL_STATIC_LIBRARIES  := \
    $(DSP_STATIC_LIBS)     \


LOCAL_MULTILIB          := $(DSP_PLAT_BITS)
LOCAL_LDFLAGS           := $(DSP_SERVER_LDFLAGS)
LOCAL_MODULE            := dspserver
LOCAL_MODULE_TAGS       := optional
#LOCAL_STRIP_MODULE     := false

ifeq ($(strip $(BUILD_DEBUG)), 1)
$(warning value of ###################################################### $(LOCAL_MODULE))
$(warning valus of               LOCAL_PATH: $(LOCAL_PATH))
$(warning valus of          LOCAL_SRC_FILES: $(LOCAL_SRC_FILES))
$(warning valus of   LOCAL_SHARED_LIBRARIES: $(LOCAL_SHARED_LIBRARIES))
$(warning valus of   LOCAL_STATIC_LIBRARIES: $(LOCAL_STATIC_LIBRARIES))
$(warning value of            LOCAL_LDFLAGS: $(LOCAL_LDFLAGS))
$(warning value of             LOCAL_CFLAGS: $(LOCAL_CFLAGS))
$(warning value of           LOCAL_CPPFLAGS: $(LOCAL_CPPFLAGS))
$(warning value of #--#-------------------------------------------------- )
endif

include $(BUILD_EXECUTABLE)
