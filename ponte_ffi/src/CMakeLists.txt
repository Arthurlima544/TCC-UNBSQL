# The Flutter tooling requires that developers have CMake 3.10 or later
# installed. You should not increase this version, as doing so will cause
# the plugin to fail to compile for some customers of the plugin.
cmake_minimum_required(VERSION 3.10)

project(ponte_ffi_library VERSION 0.0.1 LANGUAGES C)

add_library(ponte_ffi SHARED
  "ponte_ffi.c"
)

set_target_properties(ponte_ffi PROPERTIES
  PUBLIC_HEADER ponte_ffi.h
  OUTPUT_NAME "ponte_ffi"
)

target_compile_definitions(ponte_ffi PUBLIC DART_SHARED_LIB)
