include(ECMAddTests)

find_package(Qt${QT_MAJOR_VERSION} ${REQUIRED_QT_VERSION} CONFIG REQUIRED Test)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../src")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/../src")

ecm_add_tests(
    hourlyweatherforecasttest.cpp
    dailyweatherforecasttest.cpp
    weatherforecasttest.cpp
    sunrisetest.cpp
    geotimezonetest.cpp
    sunrisesourcetest.cpp
    locationquerytest.cpp
    pendingweatherforecasttest.cpp
    LINK_LIBRARIES KF5::KWeatherCore Qt${QT_MAJOR_VERSION}::Test
)

