project(parallaxoidwallpaper)

find_package(KDE4 REQUIRED)
find_package(KDE4Workspace REQUIRED)
include(KDE4Defaults)

add_definitions(${KDE4_DEFINITIONS})
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

set(parallaxoidwallpaper_SRCS
    parallaxoidwallpaper.cpp
)

kde4_add_plugin(parallaxoidwallpaper ${parallaxoidwallpaper_SRCS})
target_link_libraries(parallaxoidwallpaper ${KDE4_PLASMA_LIBS})

install(TARGETS parallaxoidwallpaper DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES parallaxoidwallpaper.desktop DESTINATION ${SERVICES_INSTALL_DIR})
