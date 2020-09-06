#ARCHS = arm64 arm64e
#TARGET = iphone:clang:11.2:11.2

TARGET = simulator:clang:11.2:7.0
ARCHS = x86_64

INSTALL_TARGET_PROCESSES = SpringBoard
include $(THEOS)/makefiles/common.mk

TWEAK_NAME = Docky
$(TWEAK_NAME)_FILES = Tweak.xm
$(TWEAK_NAME)_FRAMEWORKS += WebKit
$(TWEAK_NAME)_CFLAGS = -fobjc-arc -O3

include $(THEOS_MAKE_PATH)/tweak.mk
