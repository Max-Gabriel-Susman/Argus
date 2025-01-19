find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)

# This will populate variables like:
#   GSTREAMER_INCLUDE_DIRS, GSTREAMER_LIBRARY_DIRS, GSTREAMER_LIBRARIES
#
# You then need to add them to your target:
include_directories(${GSTREAMER_INCLUDE_DIRS})
link_directories(${GSTREAMER_LIBRARY_DIRS})

target_link_libraries(Argus ${GSTREAMER_LIBRARIES})
