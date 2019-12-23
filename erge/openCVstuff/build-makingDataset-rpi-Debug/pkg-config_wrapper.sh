#!/bin/sh
PKG_CONFIG_SYSROOT_DIR=/home/andre/buildroot-2019.08/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot
export PKG_CONFIG_SYSROOT_DIR
PKG_CONFIG_LIBDIR=/home/andre/buildroot-2019.08/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/lib/pkgconfig:/home/andre/buildroot-2019.08/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/share/pkgconfig:/home/andre/buildroot-2019.08/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/lib/arm-buildroot-linux-uclibcgnueabihf/pkgconfig
export PKG_CONFIG_LIBDIR
exec pkg-config "$@"
