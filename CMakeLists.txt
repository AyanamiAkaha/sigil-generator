cmake_minimum_required (VERSION 3.5)
project(Sigilizer)
add_executable(sigilize colors.c draw.c process_intent.c sigilize.c )

set(CMAKE_BUILD_TYPE Debug)

set (Sigilize_VERSION_MAJOR 0)
set (Sigilize_VERSION_MINOR 1)
set (Sigilize_VERSION_PATCH 0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(Cairo REQUIRED)

configure_file (
	"${PROJECT_SOURCE_DIR}/sigilize_config.h.in"
	"${PROJECT_SOURCE_DIR}/sigilize_config.h"
	)

include_directories("${PROJECT_BINARY_DIR}")
target_link_libraries(sigilize ${CAIRO_LIBRARIES} -lm)

