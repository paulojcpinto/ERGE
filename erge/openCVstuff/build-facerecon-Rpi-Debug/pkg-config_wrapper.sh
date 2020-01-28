#!/bin/sh
PKG_CONFIG_SYSROOT_DIR=/home/paulo/buildroot/buildroot-2019.02.7/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot
export PKG_CONFIG_SYSROOT_DIR
PKG_CONFIG_LIBDIR=/home/paulo/buildroot/buildroot-2019.02.7/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/lib/pkgconfig:/home/paulo/buildroot/buildroot-2019.02.7/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/share/pkgconfig:/home/paulo/buildroot/buildroot-2019.02.7/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/lib/arm-buildroot-linux-uclibcgnueabihf/pkgconfig
export PKG_CONFIG_LIBDIR
exec pkg-config "$@"
