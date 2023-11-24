include(ECMAddTests)

find_package(Qt${QT_MAJOR_VERSION} ${REQUIRED_QT_VERSION} CONFIG REQUIRED Test)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../src")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/../src")

ecm_add_tests(
    capparsertest.cpp
    metnoparsertest.cpp
    hourlyweatherforecasttest.cpp
    dailyweatherforecasttest.cpp
    weatherforecasttest.cpp
    locationquerytest.cpp
    pendingweatherforecasttest.cpp
    LINK_LIBRARIES KWeatherCore Qt6::Test
)

ecm_add_tests(geotimezonetest.cpp LINK_LIBRARIES KWeatherCore Qt6::Test Qt6::Network)
