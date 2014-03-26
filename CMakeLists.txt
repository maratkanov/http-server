cmake_minimum_required(VERSION 2.8)

project(http-server)
INCLUDE(/usr/share/cmake-2.8/Modules/FindBoost.cmake)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS thread date_time system filesystem REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
aux_source_directory(. SRC_LIST)
set(HDR_LIST request.hpp header.hpp)
add_executable(${PROJECT_NAME} ${SRC_LIST} ${HDR_LIST})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

#get_cmake_property(_variableNames VARIABLES)
#foreach(_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
