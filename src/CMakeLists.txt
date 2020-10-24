set(libkweather_SRCS
    locationquery.cpp
    locationqueryresult.cpp
)


add_library(KF5LibKWeather ${libkweather_SRCS})
generate_export_header(KF5LibKWeather BASE_NAME LibKWeather)
add_library(KF5::LibKWeather ALIAS KF5LibKWeather)

target_include_directories(KF5LibKWeather INTERFACE "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}/LibKWeather>")

target_link_libraries(KF5LibKWeather
PUBLIC Qt5::Core
PRIVATE Qt5::Network
        KF5::I18n
)

set_target_properties(KF5LibKWeather
    PROPERTIES VERSION ${LIBKWEATHER_VERSION_STRING}
    SOVERSION ${LIBKWEATHER_SOVERSION}
    EXPORT_NAME "libkweather"
)

ecm_generate_headers(LibKWeather_HEADERS
    HEADER_NAMES
        LocationQuery
        LocationQueryResult
        # other classes

    REQUIRED_HEADERS LibKWeather_HEADERS
)

# Install files
install(TARGETS KF5LibKWeather
        EXPORT KF5LibKWeatherTargets
        ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/libkweather_export.h
            ${LibKWeather_HEADERS}
        DESTINATION ${INCLUDE_INSTALL_DIR}/LibKWeather
        COMPONENT Devel)

include(ECMGeneratePriFile)
ecm_generate_pri_file(
    BASE_NAME LibKWeather
    LIB_NAME KF5LibKWeather
    DEPS "core"
    FILENAME_VAR PRI_FILENAME
)
install(FILES ${PRI_FILENAME}
        DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
