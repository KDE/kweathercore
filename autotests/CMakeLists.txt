include(ECMAddTests)

find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Test)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../src")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/../src")

ecm_add_tests(
    hourlyforecasttest.cpp
    dailyforecasttest.cpp
    weatherforecasttest.cpp
    sunrisetest.cpp
    geotimezonetest.cpp
    sunrisesourcetest.cpp
    locationquerytest.cpp
    LINK_LIBRARIES KF5::KWeatherCore Qt5::Test
)

