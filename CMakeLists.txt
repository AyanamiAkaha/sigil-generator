cmake_minimum_required (VERSION 3.5)
project(Sigilizer)
add_executable(sigilize sigilize.c)

set (Sigilize_VERSION_MAJOR 0)
set (Sigilize_VERSION_MINOR 1)
set (Sigilize_VERSION_PATCH 0)

find_package(cairo REQUIRED)

configure_file (
	"${PROJECT_SOURCE_DIR}/sigilize_config.h.in"
	"${PROJECT_SOURCE_DIR}/sigilize_config.h"
	)

include_directories("${PROJECT_BINARY_DIR}")
