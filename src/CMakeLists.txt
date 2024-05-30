set(kweathercore_LIB_SRCS
    locationquery.cpp
    locationquery.h
    locationqueryreply.cpp
    locationqueryreply.h
    locationqueryresult.cpp
    locationqueryresult.h
    weatherforecastsource.cpp
    weatherforecastsource.h
    weatherforecast.cpp
    weatherforecast.h
    hourlyweatherforecast.cpp
    hourlyweatherforecast.h
    dailyweatherforecast.cpp
    dailyweatherforecast.h
    geotimezone.cpp
    geotimezone.h
    kweathercore.cpp
    kweathercore_p.h
    metnoparser.cpp
    metnoparser_p.h
    pendingweatherforecast.cpp
    pendingweatherforecast.h
    pendingweatherforecast_p.h
    capalertinfo.cpp
    capalertinfo.h
    capalertmessage.cpp
    capalertmessage.h
    capparser.cpp
    capparser.h
    caparea.cpp
    caparea.h
    capreference.cpp
    capreference.h
    reply.cpp
    reply.h
)


add_library(KWeatherCore ${kweathercore_LIB_SRCS})
generate_export_header(KWeatherCore BASE_NAME KWeatherCore EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/kweathercore/kweathercore_export.h)
set(kweathercore_BUILD_INCLUDE_DIRS ${KWeatherCore_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(KWeatherCore PUBLIC "$<BUILD_INTERFACE:${kweathercore_BUILD_INCLUDE_DIRS}>")
target_include_directories(KWeatherCore INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KWeatherCore>")

target_link_libraries(KWeatherCore
PUBLIC
    Qt6::Core
PRIVATE
    Qt6::Network
    Qt6::Positioning
    KF6::I18n
    KF6::I18nLocaleData
    KF6::Holidays
)

set_target_properties(KWeatherCore PROPERTIES VERSION ${KWEATHERCORE_VERSION}
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
    CAPParser
    CAPArea
    CAPNamedValue
    CAPReference
    Reply

    PREFIX KWeatherCore
    REQUIRED_HEADERS KWeatherCore_HEADERS
)
install(FILES ${KWeatherCore_CamelCase_HEADERS} DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KWeatherCore/KWeatherCore COMPONENT Devel)

install(TARGETS KWeatherCore EXPORT KWeatherCoreTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/kweathercore/kweathercore_export.h
  ${KWeatherCore_HEADERS}
  DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KWeatherCore/kweathercore COMPONENT Devel
)

if(BUILD_QCH)
    ecm_add_qch(
        KWeatherCore_QCH
        NAME KWeatherCore
        BASE_NAME KWeatherCore
        VERSION ${PROJECT_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${KWeatherCore_HEADERS}
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        LINK_QCHS
            Qt6Core_QCH
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
    LIB_NAME KWeatherCore
    DEPS "core"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR}/KWeatherCore
)
install(FILES ${PRI_FILENAME}
        DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
