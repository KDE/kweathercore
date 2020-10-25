cmake_minimum_required(VERSION 3.5)
set(KF5_VERSION "5.76.0") # handled by release scripts
set(KF5_DEP_VERSION "5.75.0") # handled by release scripts
project(KWeatherCore VERSION ${KF5_VERSION})

include(FeatureSummary)
find_package(ECM ${KF5_DEP_VERSION}  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(GenerateExportHeader)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMAddQch)

ecm_setup_version(PROJECT VARIABLE_PREFIX KWeatherCore
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kweathercore_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5KWeatherCoreConfigVersion.cmake"
                        SOVERSION 5)
# Dependencies
set(REQUIRED_QT_VERSION 5.12.0)

# Required Qt5 components to build this framework
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Core Network)

# TODO: Add all required find_package() calls here. For example:
# find_package(KF5CoreAddons ${KF5_VERSION} REQUIRED)

find_package(KF5I18n ${KF5_DEP_VERSION} REQUIRED)



# Translations
add_definitions(-DTRANSLATION_DOMAIN=\"KWeatherCore5\")
if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    ki18n_install(po)


endif()

# Subdirectories
add_subdirectory(src)
add_subdirectory(autotests)
add_subdirectory(tests)

# Create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KF5KWeatherCore")

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KF5KWeatherCoreConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5KWeatherCoreConfig.cmake"
    INSTALL_DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
)

install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/KF5KWeatherCoreConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/KF5KWeatherCoreConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel)

install(EXPORT KF5KWeatherCoreTargets
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        FILE KF5KWeatherCoreTargets.cmake
        NAMESPACE KF5::)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/kweathercore_version.h"
        DESTINATION "${INCLUDE_INSTALL_DIR}"
        COMPONENT Devel)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
