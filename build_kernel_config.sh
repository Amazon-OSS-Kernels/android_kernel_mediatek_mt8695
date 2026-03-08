################################################################################
#
#  build_kernel_config.sh
#
#  Copyright (c) 2016-2022 Amazon.com, Inc. or its affiliates. All Rights Reserved.
#
################################################################################

KERNEL_SUBPATH="kernel/mediatek/mt8695/4.4"
DEFCONFIG_NAME="mantis_defconfig"
TARGET_ARCH="arm"
TOOLCHAIN_PREFIX="arm-eabi-"
MAKE_DTBS=y

# Expected image files are seperated with ":"
KERNEL_IMAGES="arch/arm/boot/Image:arch/arm/boot/zImage:arch/arm/boot/zImage-dtb"

################################################################################
# NOTE: You must fill in the following with the path to a copy of
#       arm-eabi-4.8 compiler.
################################################################################
CROSS_COMPILER_PATH=""
