include $(call all-subdir-makefiles)

#for ndk coverity
ifneq ($(strip $(PLATFORM_ROOT)),)
include $(PLATFORM_ROOT)platform_lib/Android.mk
endif
