include_directories(${CMAKE_SOURCE_DIR}/include/)

add_executable(test_basic_shapes test_basic_shapes.cpp)
target_link_libraries(test_basic_shapes gfx)

add_executable(test_plot test_plot.cpp)
target_link_libraries(test_plot gfx)

IF( BUILD_GFX_QT_SURFACE )
find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-3.0) # look into FindPkgConfig.cmake,
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})
add_executable(test_qt_surface test_qt_surface.cpp)
target_link_libraries(test_qt_surface gfx ${GTKMM_LIBRARIES} pthread)

ENDIF()
