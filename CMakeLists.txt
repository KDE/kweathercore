cmake_minimum_required(VERSION 3.16)

# KDE Application Version, managed by release script
set (RELEASE_SERVICE_VERSION_MAJOR "25")
set (RELEASE_SERVICE_VERSION_MINOR "12")
set (RELEASE_SERVICE_VERSION_MICRO "0")
set (RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")
project(KWeatherCore VERSION ${RELEASE_SERVICE_VERSION})

set(CMAKE_CXX_STANDARD 20)

include(FeatureSummary)

set(REQUIRED_QT_VERSION 6.5.0)
set(KF_DEP_VERSION "6.3.0")

find_package(ECM ${KF_DEP_VERSION}  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://invent.kde.org/frameworks/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(KDEClangFormat)
include(KDEGitCommitHooks)
include(ECMDeprecationSettings)
include(ECMQmlModule)

include(GenerateExportHeader)

include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMAddQch)

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

ecm_setup_version(PROJECT VARIABLE_PREFIX KWEATHERCORE
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kweathercore_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KWeatherCoreConfigVersion.cmake"
                        SOVERSION 6)

find_package(Qt6 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Core Network Positioning Qml)
find_package(KF6 ${KF_DEP_VERSION} REQUIRED COMPONENTS I18n Holidays)


add_definitions(-DTRANSLATION_DOMAIN=\"kweathercore6\")
if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    ki18n_install(po)
endif()
ecm_set_disabled_deprecation_versions(QT 6.8.0
    KF 6.12.0
)

add_definitions(-DQT_NO_FOREACH)
add_subdirectory(src)
add_subdirectory(src/qml)

option(BUILD_EXAMPLES "Build example applications" OFF)

if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KWeatherCore")

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KWeatherCore_QCH
        FILE KWeatherCoreQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KWeatherCoreQchTargets.cmake\")")
endif()

include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KWeatherCoreConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KWeatherCoreConfig.cmake"
    INSTALL_DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
)

install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/KWeatherCoreConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/KWeatherCoreConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
)

install(EXPORT KWeatherCoreTargets
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        FILE KWeatherCoreTargets.cmake)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kweathercore_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR} COMPONENT Devel
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)

