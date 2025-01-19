cmake_minimum_required(VERSION 3.10)

project(Argus)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(Argus 
    Argus/Argus.cpp 
    Argus/Argus.h
)



if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET Argus PROPERTY CXX_STANDARD 20)
endif()

# -- Add GStreamer, e.g. method A with pkg-config --
find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
include_directories(${GSTREAMER_INCLUDE_DIRS})
link_directories(${GSTREAMER_LIBRARY_DIRS})
target_link_libraries(Argus ${GSTREAMER_LIBRARIES})

include_directories(${GST_INCLUDE_DIRS})


# or -- method B on Windows, set GSTREAMER_ROOT, etc. --
# set(GSTREAMER_ROOT "C:/gstreamer/1.0/x86_64")
# ... and so on ...
