set(kweathercore_LIB_SRCS
    locationquery.cpp
    locationqueryreply.cpp
    locationqueryresult.cpp
    weatherforecastsource.cpp
    weatherforecast.cpp
    hourlyweatherforecast.cpp
    dailyweatherforecast.cpp
    geotimezone.cpp
    kweathercore.cpp
    metnoparser.cpp
    pendingweatherforecast.cpp
    capalertinfo.cpp
    alertfeedentry.cpp
    capalertmessage.cpp
    alertmanager.cpp
    feedparser.cpp
    pendingalerts.cpp
    pendingcap.cpp
    capparser.cpp
    caparea.cpp
    capreference.cpp
    reply.cpp
)


add_library(KF5KWeatherCore ${kweathercore_LIB_SRCS})
generate_export_header(KF5KWeatherCore BASE_NAME KWeatherCore EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/kweathercore/kweathercore_export.h)
add_library(KF5::KWeatherCore ALIAS KF5KWeatherCore)
set(kweathercore_BUILD_INCLUDE_DIRS ${KWeatherCore_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(KF5KWeatherCore PUBLIC "$<BUILD_INTERFACE:${kweathercore_BUILD_INCLUDE_DIRS}>")
target_include_directories(KF5KWeatherCore INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/KWeatherCore>")

target_link_libraries(KF5KWeatherCore
PUBLIC
    Qt${QT_MAJOR_VERSION}::Core
PRIVATE
    Qt${QT_MAJOR_VERSION}::Network
    Qt${QT_MAJOR_VERSION}::Positioning
    KF5::I18n
    KF5::Holidays
)

set_target_properties(KF5KWeatherCore PROPERTIES VERSION ${KWEATHERCORE_VERSION_STRING}
                                                 SOVERSION ${KWEATHERCORE_SOVERSION}
                                                 EXPORT_NAME KWeatherCore
)

ecm_generate_headers(KWeatherCore_CamelCase_HEADERS
    HEADER_NAMES
    LocationQuery
    LocationQueryReply
    LocationQueryResult
    DailyWeatherForecast
    HourlyWeatherForecast
    PendingWeatherForecast
    WeatherForecast
    WeatherForecastSource
    GeoTimezone
    CAPAlertInfo
    CAPAlertMessage
    AlertFeedEntry
    AlertManager
    PendingAlerts
    PendingCAP
    CAPParser
    CAPArea
    CAPNamedValue
    CAPReference
    Reply

    PREFIX KWeatherCore
    REQUIRED_HEADERS KWeatherCore_HEADERS
)
install(FILES ${KWeatherCore_CamelCase_HEADERS} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KWeatherCore/KWeatherCore COMPONENT Devel)

install(TARGETS KF5KWeatherCore EXPORT KF5KWeatherCoreTargets ${KF_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/kweathercore/kweathercore_export.h
  ${KWeatherCore_HEADERS}
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KWeatherCore/kweathercore COMPONENT Devel
)

if(BUILD_QCH)
    ecm_add_qch(
        KF5KWeatherCore_QCH
        NAME KWeatherCore
        BASE_NAME KF5KWeatherCore
        VERSION ${KF5_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${KWeatherCore_HEADERS}
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        LINK_QCHS
            Qt5Core_QCH
        INCLUDE_DIRS
            ${kweathercore_BUILD_INCLUDE_DIRS}
        BLANK_MACROS
            KWEATHERCORE_EXPORT
            KWEATHERCORE_DEPRECATED
            KWEATHERCORE_DEPRECATED_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
        )
endif()

include(ECMGeneratePriFile)
ecm_generate_pri_file(
    BASE_NAME KWeatherCore
    LIB_NAME KF5KWeatherCore
    DEPS "core"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF}/KWeatherCore
)
install(FILES ${PRI_FILENAME}
        DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
