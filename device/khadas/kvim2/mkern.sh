#!/bin/bash -ex

# Run from top of kitkat source

#ROOTFS=$1
ROOTFS="out/target/product/kvim2/ramdisk.img"
PREFIX_CROSS_COMPILE=/opt/toolchains/gcc-linaro-4.9.4-2017.01-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-

if [ "$ROOTFS" == "" -o ! -f "$ROOTFS" ]; then
    echo "Usage: $0 <ramdisk.img> [m]"
    exit 1
fi

KERNEL_OUT=out/target/product/kvim2/obj/KERNEL_OBJ
#mkdir -p $KERNEL_OUT

if [ ! -f $KERNEL_OUT/.config ]; then
    make -C common O=../$KERNEL_OUT kvim2_defconfig ARCH=arm64 CROSS_COMPILE=$PREFIX_CROSS_COMPILE
fi
make -C common O=../$KERNEL_OUT ARCH=arm64 -j6 CROSS_COMPILE=$PREFIX_CROSS_COMPILE modules Image.gz

if [ "$2" != "m" ]; then
#    make -C common O=../$KERNEL_OUT kvim2.dtd ARCH=arm64 CROSS_COMPILE=$PREFIX_CROSS_COMPILE
    make -C common O=../$KERNEL_OUT kvim2.dtb ARCH=arm64 CROSS_COMPILE=$PREFIX_CROSS_COMPILE PARTITION_DTSI=partition_mbox.dtsi
fi


if [ "$2" != "m" ]; then
    out/host/linux-x86/bin/mkbootimg --kernel common/../$KERNEL_OUT/arch/arm64/boot/Image.gz \
        --base 0x0 \
        --kernel_offset 0x1080000 \
        --ramdisk ${ROOTFS} \
        --output ./out/target/product/kvim2/boot.img
    ls -l ./out/target/product/kvim2/boot.img
    echo "boot.img done"
fi
