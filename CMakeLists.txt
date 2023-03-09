cmake_minimum_required(VERSION 3.5)
project(KWeatherCore VERSION "0.7.0")

set(CMAKE_CXX_STANDARD 17)

include(FeatureSummary)

set(REQUIRED_QT_VERSION 5.12.0)
set(KF_DEP_VERSION "5.75.0")

find_package(ECM ${KF_DEP_VERSION}  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(KDEClangFormat)
include(KDEGitCommitHooks)

include(GenerateExportHeader)

include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMAddQch)

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

ecm_setup_version(PROJECT VARIABLE_PREFIX KWEATHERCORE
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kweathercore_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF${QT_MAJOR_VERSION}KWeatherCoreConfigVersion.cmake"
                        SOVERSION 5)

find_package(Qt${QT_MAJOR_VERSION} ${REQUIRED_QT_VERSION} CONFIG REQUIRED Core Network Positioning)
find_package(KF${QT_MAJOR_VERSION} ${KF_DEP_VERSION} REQUIRED COMPONENTS I18n Holidays)


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
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF${QT_MAJOR_VERSION}KWeatherCore")

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KF${QT_MAJOR_VERSION}KWeatherCore_QCH
        FILE KF${QT_MAJOR_VERSION}KWeatherCoreQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF${QT_MAJOR_VERSION}KWeatherCoreQchTargets.cmake\")")
endif()

include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KFKWeatherCoreConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF${QT_MAJOR_VERSION}KWeatherCoreConfig.cmake"
    INSTALL_DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
)

install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/KF${QT_MAJOR_VERSION}KWeatherCoreConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/KF${QT_MAJOR_VERSION}KWeatherCoreConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
)

install(EXPORT KF${QT_MAJOR_VERSION}KWeatherCoreTargets
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        FILE KF${QT_MAJOR_VERSION}KWeatherCoreTargets.cmake
        NAMESPACE KF${QT_MAJOR_VERSION}::)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kweathercore_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF} COMPONENT Devel
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)

