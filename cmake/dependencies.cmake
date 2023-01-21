find_package(Cairo REQUIRED)
find_package(Fontconfig REQUIRED)
find_package(Freetype REQUIRED)
find_package(HarfBuzz REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc i18n)

option(BUILD_GFX_QT_SURFACE "Build QtPainter graphics surface" OFF)

IF( BUILD_GFX_QT_SURFACE )
find_package(Qt5Widgets 5.5 CONFIG REQUIRED)
SET(QT_LIBRARIES Qt5::Widgets)
ENDIF ( BUILD_GFX_QT_SURFACE )
