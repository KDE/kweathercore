cmake_minimum_required(VERSION 3.5)
set(KF5_VERSION "0.1.0") # handled by release scripts
set(KF5_DEP_VERSION "5.75.0") # handled by release scripts
project(KWeatherCore VERSION ${KF5_VERSION})

include(FeatureSummary)
find_package(ECM ${KF5_DEP_VERSION}  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)


set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

set(REQUIRED_QT_VERSION 5.12.0)

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)


include(GenerateExportHeader)

include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMAddQch)

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

ecm_setup_version(PROJECT VARIABLE_PREFIX KWEATHERCORE
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kweathercore_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5KWeatherCoreConfigVersion.cmake"
                        SOVERSION 5)

find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Core Network Positioning)
find_package(KF5I18n ${KF5_DEP_VERSION} REQUIRED)


add_definitions(-DTRANSLATION_DOMAIN=\"kweathercore5\")
if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    ki18n_install(po)
endif()
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050d00)
add_definitions(-DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x054B00)
add_definitions(-DQT_NO_FOREACH)
add_subdirectory(src)

option(BUILD_EXAMPLES "Build example applications" OFF)

if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5KWeatherCore")

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KF5KWeatherCore_QCH
        FILE KF5KWeatherCoreQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5KWeatherCoreQchTargets.cmake\")")
endif()

include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KF5KWeatherCoreConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5KWeatherCoreConfig.cmake"
    INSTALL_DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
)

install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/KF5KWeatherCoreConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/KF5KWeatherCoreConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
)

install(EXPORT KF5KWeatherCoreTargets
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        FILE KF5KWeatherCoreTargets.cmake
        NAMESPACE KF5::)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kweathercore_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5} COMPONENT Devel
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
