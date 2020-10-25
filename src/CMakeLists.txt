set(kweathercore_SRCS
    locationquery.cpp
    locationqueryresult.cpp
    weatherforecast.cpp
)


add_library(KF5KWeatherCore ${kweathercore_SRCS})
generate_export_header(KF5KWeatherCore BASE_NAME KWeatherCore)
add_library(KF5::KWeatherCore ALIAS KF5KWeatherCore)

target_include_directories(KF5KWeatherCore INTERFACE "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}/KWeatherCore>")

target_link_libraries(KF5KWeatherCore
PUBLIC Qt5::Core
PRIVATE Qt5::Network
        Qt5::Positioning
        KF5::I18n
)

set_target_properties(KF5KWeatherCore
    PROPERTIES VERSION ${LIBKWEATHER_VERSION_STRING}
    SOVERSION ${LIBKWEATHER_SOVERSION}
    EXPORT_NAME "kweathercore"
)

ecm_generate_headers(KWeatherCore_HEADERS
    HEADER_NAMES
        LocationQuery
        LocationQueryResult
        # other classes

    REQUIRED_HEADERS KWeatherCore_HEADERS
)

# Install files
install(TARGETS KF5KWeatherCore
        EXPORT KF5KWeatherCoreTargets
        ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/kweathercore_export.h
            ${KWeatherCore_HEADERS}
        DESTINATION ${INCLUDE_INSTALL_DIR}/KWeatherCore
        COMPONENT Devel)

include(ECMGeneratePriFile)
ecm_generate_pri_file(
    BASE_NAME KWeatherCore
    LIB_NAME KF5KWeatherCore
    DEPS "core"
    FILENAME_VAR PRI_FILENAME
)
install(FILES ${PRI_FILENAME}
        DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
