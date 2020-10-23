find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Test)

include(ECMMarkAsTest)

macro(libkweather_tests)
    foreach(_testname ${ARGN})
        add_executable(${_testname} ${_testname}.cpp)
        target_link_libraries(${_testname} Qt5::Test KF5LibKWeather)
        add_test(libkweather-${_testname} ${_testname})
        ecm_mark_as_test(${_testname})
    endforeach(_testname)
endmacro()

#libkweather_tests(
#    libkweathertest
#)
