

set(LIB_SOURCES
    path.cpp
    pen.cpp
    color.cpp
    image.cpp
    text_layout.cpp

    impl/canvas.cpp
    impl/text_layout_engine.cpp
    impl/text_layout_engine.hpp
    impl/font_manager.cpp
    impl/font_manager.hpp
    impl/scrptrun.cpp
    impl/text_path.cpp
    impl/surface.cpp
    impl/exception.cpp

    svg/svg_parser.cpp
    svg/svg_parser.hpp
    svg/svg_document.cpp
    svg/svg_dom.cpp
    svg/svg_length.cpp
    svg/svg_length.hpp
    svg/svg_style.cpp
    svg/svg_style.hpp
    svg/svg_style_attributes.hpp
    svg/svg_parse_util.cpp
    svg/svg_parse_util.hpp
    svg/svg_render_context.cpp
    svg/svg_render_context.hpp
    svg/dictionary.cpp
    svg/dictionary.hpp
    svg/base64.cpp
    svg/base64.hpp

    plot/plot.cpp
    plot/line_plot.cpp
    plot/bar_chart.cpp
    plot/error_bars.cpp
    plot/raster_plot.cpp
    plot/axis.cpp
    plot/ticks.cpp
    plot/legend.cpp
    plot/markers.cpp
    plot/colormap.cpp
    plot/color_ramp.cpp
    plot/gtk_widget.cpp
    plot/gtk_widget.hpp

    ${LIBRARY_INCLUDE_DIR}/impl/canvas.hpp
    ${LIBRARY_INCLUDE_DIR}/font.hpp
    ${LIBRARY_INCLUDE_DIR}/pen.hpp
    ${LIBRARY_INCLUDE_DIR}/brush.hpp
    ${LIBRARY_INCLUDE_DIR}/path.hpp
    ${LIBRARY_INCLUDE_DIR}/canvas.hpp
    ${LIBRARY_INCLUDE_DIR}/image.hpp
    ${LIBRARY_INCLUDE_DIR}/vector.hpp
    ${LIBRARY_INCLUDE_DIR}/rectangle.hpp
    ${LIBRARY_INCLUDE_DIR}/xform.hpp
    ${LIBRARY_INCLUDE_DIR}/text_layout.hpp
    ${LIBRARY_INCLUDE_DIR}/glyph.hpp
    ${LIBRARY_INCLUDE_DIR}/svg_document.hpp

    ${LIBRARY_INCLUDE_DIR}/plot/plot.hpp
    ${LIBRARY_INCLUDE_DIR}/plot/axis.hpp
    ${LIBRARY_INCLUDE_DIR}/plot/ticks.hpp
    ${LIBRARY_INCLUDE_DIR}/plot/legend.hpp
    ${LIBRARY_INCLUDE_DIR}/plot/plot_element.hpp
    ${LIBRARY_INCLUDE_DIR}/plot/line_plot.hpp
    ${LIBRARY_INCLUDE_DIR}/plot/bar_chart.hpp
    ${LIBRARY_INCLUDE_DIR}/plot/error_bars.hpp
    ${LIBRARY_INCLUDE_DIR}/plot/raster_plot.hpp

)

IF ( BUILD_GFX_QT_SURFACE )
LIST(APPEND LIB_SOURCES qt_surface.cpp)
ENDIF()

add_library(gfx ${LIBRARY_TYPE} ${LIB_SOURCES} )


target_include_directories(gfx PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include> PRIVATE ${CMAKE_SOURCE_DIR}/src)


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    ${CAIRO_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
    ${HARFBUZZ_INCLUDE_DIRS}
    ${FONTCONFIG_INCLUDE_DIR}
)

target_link_libraries(gfx
    PRIVATE
    ${CAIRO_LIBRARIES}
    ${HARFBUZZ_LIBRARIES}
    ${FONTCONFIG_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    ${ICU_LIBRARIES}
    ${QT_LIBRARIES}
    ${GTKMM_LIBRARIES}
    fmt::fmt-header-only
)

# Install library
install(TARGETS gfx
  EXPORT ${PROJECT_NAME}-export
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR}" COMPONENT stlib
  COMPONENT dev)


# Install headers
install(DIRECTORY "${LIBRARY_INCLUDE_DIR}" DESTINATION "${INSTALL_INCLUDE_DIR}/gfx" )


