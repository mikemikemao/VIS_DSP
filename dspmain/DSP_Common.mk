#define DSP common
DSP_TOP := $(call my-dir)

DSP_SRC_TOP  := $(DSP_TOP)/src
DSP_APP_TOP  := $(DSP_SRC_TOP)/apps
DSP_FRM_TOP  := $(DSP_SRC_TOP)/framework
DSP_PLAT_TOP := $(DSP_SRC_TOP)/platform
DSP_COM_TOP  := $(DSP_SRC_TOP)/common
DSP_SERV_TOP := $(DSP_SRC_TOP)/service

# DSP platform bits (set: 32 or 64)
DSP_PLAT_BITS  := 64
DSP_USE_IPC    := 1
BUILD_DEBUG    := 1

AI_SVR_FLAG    := 1
AI_DFR_FLAG    := 1

DSP_LIBS_DIR   :=$(DSP_TOP)/lib/lib
DSP_RUN_LIBS_DIR := /hik/dsp/lib/

#include
DSP_INC      :=                         \


DSP_APP_INC  :=                         \


LOCAL_PROPRIETARY_MODULE := false

LOCAL_HEADER_LIBRARIES += \


DSP_PLAT_INC :=                          \
                $(DSP_PLAT_TOP)                      \


DSP_COM_INC :=                                   \
                $(DSP_COM_TOP)                               \
                $(DSP_COM_TOP)/sal                           \


DSP_FRM_INC :=                                   \
                $(DSP_FRM_TOP)                               \

#strip 这个关键字的作用是去掉对象中开头和结尾的空字符，包括空格，Tab缩进等不可见字符
ifeq ($(strip $(AI_SVR_FLAG)),1)

else

endif

ifeq ($(strip $(AI_DFR_FLAG)),1)

else
   
endif

DSP_ISP_INC :=                                                        \


DSP_BASE_INC :=                                                       \
    $(PLATFORM_ROOT)external/libcxx/include                           \
    $(PLATFORM_ROOT)system/core/include/cutils                        \
    $(PLATFORM_ROOT)system/core/include/system                        \
    $(PLATFORM_ROOT)system/core/libutils/include                      \
    $(PLATFORM_ROOT)system/core/liblog/include                        \
    $(PLATFORM_ROOT)system/core/libsync/include                       \
    $(PLATFORM_ROOT)hardware/libhardware/include/hardware             \
    $(PLATFORM_ROOT)frameworks/native/include/ui                      \
    $(PLATFORM_ROOT)hardware/rockchip/librkvpu                        \
    $(PLATFORM_ROOT)hardware/rockchip/librkvpu/common/vpu_mem_pool    \
    $(PLATFORM_ROOT)hardware/rockchip/librga                          \
    $(PLATFORM_ROOT)external/libdrm                                   \
    $(PLATFORM_ROOT)external \
    $(PLATFORM_ROOT)external/libdrm/include/drm                       \
    $(PLATFORM_ROOT)external/mesa3d/include                           \
    $(PLATFORM_ROOT)frameworks/native/libs/ui/include/ui

DSP_SERVICE_INC :=                                                    \
    $(DSP_SERV_TOP)/include                                           \
    $(DSP_SERV_TOP)/service_java/native                               \
    $(DSP_SERV_TOP)/service_hidl/native                               \
    $(DSP_SERV_TOP)/service_heop                                      \
    $(DSP_SERV_TOP)/service_heop/native                               \
    $(DSP_SERV_TOP)/service_cmd

# API 29 -> Android 10.0
ifneq (1,$(strip $(shell expr $(PLATFORM_SDK_VERSION) \< 29)))

ifneq (,$(filter mali-tDVx mali-G52, $(TARGET_BOARD_PLATFORM_GPU)))
DSP_BASE_INC +=                                \
    hardware/rockchip/libgralloc/bifrost       \
    hardware/rockchip/libgralloc/bifrost/src
endif

ifneq (,$(filter mali-t860 mali-t760, $(TARGET_BOARD_PLATFORM_GPU)))
DSP_BASE_INC += hardware/rockchip/libgralloc/midgard
endif

ifneq (,$(filter mali400 mali450, $(TARGET_BOARD_PLATFORM_GPU)))
DSP_BASE_INC += hardware/rockchip/libgralloc/utgard
endif

else
DSP_BASE_INC += hardware/rockchip/libgralloc
endif

ANDROID_INC :=                                                \
    $(DSP_TOP)/display                                        \
    $(PLATFORM_ROOT)external/skia/include/core                \
    $(PLATFORM_ROOT)frameworks/av/include/media/stagefright   \
    $(PLATFORM_ROOT)frameworks/av                             \
    $(PLATFORM_ROOT)frameworks/native/include/media/openmax

#for jpeg
DJPEG_PATH  := $(PLATFORM_ROOT)hardware/rockchip/jpeghw

DSP_BASE_INC +=                                           \
    $(PLATFORM_ROOT)external/skia                         \
    $(PLATFORM_ROOT)external/skia/android                 \
    $(PLATFORM_ROOT)external/skia/include/config          \
    $(PLATFORM_ROOT)external/skia/include/core            \
    $(PLATFORM_ROOT)external/skia/include/effects         \
    $(PLATFORM_ROOT)external/skia/include/images          \
    $(PLATFORM_ROOT)external/skia/src/ports               \
    $(PLATFORM_ROOT)external/skia/include/utils           \
    $(PLATFORM_ROOT)external/expat/lib                    \
    $(DJPEG_PATH)/release/encode_release  \
    $(DJPEG_PATH)/release/decoder_release \
    $(DJPEG_PATH)/src_dec/common          \
    $(DJPEG_PATH)/src_dec/inc

#for log4j

DSP_LOG4J_INC :=	\
	$(DSP_COM_TOP)/log4j                \
	$(DSP_TOP)/include/log4j_inc        
	
#for audio
DSP_BASE_INC +=                                            \
    $(PLATFORM_ROOT)hardware/rockchip/audio                \
    $(PLATFORM_ROOT)external/sonic                         \
    $(PLATFORM_ROOT)external/tinyalsa/include              \
    $(call include-path-for, audio-utils)                  \
    $(call include-path-for, audio-route)


DSP_INC_MODULES :=    \
    $(DSP_BASE_INC)   \
    $(DSP_INC)        \
    $(DSP_APP_INC)    \
    $(DSP_COM_INC)    \
    $(DSP_PLAT_INC)   \
    $(DSP_FRM_INC)    \
    $(DSP_ISP_INC)    \
    $(ANDROID_INC)    \
    $(DSP_SERVICE_INC) \

DSP_ANDROID_BASE_SHARED_LIBS := liblog libcutils libutils libsync

DSP_SERVER_NDK_SHARED_LIBS :=   \
    libbinder                   \
    libhardware                 \
    libui                       \
    libgui                      \
    libexpat                    \
    libhardware_legacy          \
    libtinyalsa                 \
    libhidlbase                 \
    libhidlmemory               \
    android.hidl.allocator@1.0

#Android11 later
ifneq (1,$(strip $(shell expr $(PLATFORM_SDK_VERSION) \>= 30)))
DSP_SERVER_NDK_SHARED_LIBS += libhidltransport
endif

#opengl lib
DSP_SERVER_NDK_SHARED_LIBS += \
    libEGL                    \
    libGLESv2                 \
    libhwui                   \
    libdrm


DSP_SERVER_LDFLAGS   := -Wl,--rpath,$(DSP_RUN_LIBS_DIR)


ifeq ($(strip $(AI_DFR_FLAG)),1)

else
endif

ifeq ($(strip $(DSP_BUILD_USE_ASAN)), 1)
# // asan  when build command: ./bulid.sh make
# export ASAN_SYMBOLIZER_PATH=/usr/local/bin/llvm-symbolizer
# export ASAN_OPTIONS=symbolize=1:detect_stack_use_after_return=1:malloc_context_size=15:handle_segv=1:quarantine_size_mb=200:fast_unwind_on_fatal=1:fast_unwind_on_check=1:fast_unwind_on_malloc=1
# /home/app/bin/dspMediaServer

    DSP_SERVER_LDFLAGS   += $(DSP_LIBS_DIR)$(DSP_PLAT_BITS)/asan_lib/libclang_rt.asan-aarch64-android.so
endif


DSP_DEMO_LDFLAGS   += $(DSP_LIBS_DIR)$(DSP_PLAT_BITS)/log4j_lib/libbsp_log4j.a


DSP_BOUNDSCHECK_LDFLAGS  := $(DSP_LIBS_DIR)$(DSP_PLAT_BITS)/boundscheck_lib/libboundscheck.so

DSP_CFLAGS :=                           \
    -Wno-error=date-time                \
    -Wno-date-time                      \
    -Wno-typedef-redefinition           \
    -Wno-unused-variable                \
    -Wno-unused-value                   \
    -Wno-unused-parameter               \
    -Wno-missing-field-initializers     \
    -Wno-unused-comparison              \
    -Wno-unused-function                \
    -Wno-macro-redefined                \
    -Wno-int-conversion                 \
    -Wno-uninitialized                  \
    -Wno-int-to-pointer-cast            \
    -Wno-enum-conversion                \
    -Wno-pointer-sign                   \
    -Wno-writable-strings               \
    -Wno-deprecated-declarations        \
    -Wno-sign-compare                   \
    -Wno-comment


DSP_CFLAGS +=         \
    -DLINUX           \
    -DHAL_MOCKUP      \
    -DANDROID_5_X     \
    -DANDROID_7_X     \
    -DHAS_STDINT_H    \
    -D_HIK_COMPLEX_MEM_  \
    -DMALI_PRODUCT_ID_T86X=1  \
    -DMALI_AFBC_GRALLOC=1

#Android11 later
ifeq (1,$(strip $(shell expr $(PLATFORM_SDK_VERSION) \>= 30)))
DSP_CFLAGS += -DBINDER_NEW_API
endif

# TARGET_RK_GRALLOC_VERSION equal to 4 since Android 11 SDK
ifeq ($(TARGET_RK_GRALLOC_VERSION), 4)
DSP_GRALLOC_LIB4 += \
    android.hardware.graphics.mapper@4.0 \
    libgralloctypes \
    libhidlbase
DSP_CFLAGS +=  -DUSE_GRALLOC_4
endif

