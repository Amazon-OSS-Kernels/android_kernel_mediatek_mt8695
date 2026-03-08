################################################################################
#
#  build_kernel_config.sh
#
#  Copyright (c) 2020-2022  Amazon.com, Inc. or its affiliates. All Rights Reserved.
#
################################################################################

KERNEL_SUBPATH="kernel/mediatek/mt8695/4.4"
DEFCONFIG_NAME="sheldon_defconfig"
TARGET_ARCH="arm"
TOOLCHAIN_REPO="https://android.googlesource.com/platform/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9"
TOOLCHAIN_PREFIX="arm-eabi-"
MAKE_DTBS=y

# Expected image files are seperated with ":"
KERNEL_IMAGES="arch/arm/boot/Image:arch/arm/boot/zImage"

################################################################################
# NOTE: You must fill in the following with the path to a copy of
#       arm-eabi-4.9 compiler.
################################################################################
CROSS_COMPILER_PATH=""
