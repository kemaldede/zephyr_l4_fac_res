# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(zephyr_sec_lev4_rssi)


target_sources(app PRIVATE src/main.c
                            src/services/custom_service.c
                            src/utils/rssi.c)


zephyr_library_include_directories(src)