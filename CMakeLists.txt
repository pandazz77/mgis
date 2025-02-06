cmake_minimum_required(VERSION 3.5)
project(mgis)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SRCS
    src/Features.cpp
    src/Projection.cpp
    
    src/Stylers.cpp
    src/MapView.cpp
)

set(HDRS
    include/GeoTypes.hpp
    include/Features.h
    include/Projection.h

    include/Stylers.h
    include/MapView.h
)

find_package(Qt6 COMPONENTS 
    Core 
    Widgets 
REQUIRED)

SET(LIBS 
    Qt6::Core
    Qt6::Widgets
)

add_library(${PROJECT_NAME} ${SHARED} ${SRCS} ${HDRS})
target_link_libraries(${PROJECT_NAME} ${LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC include/)

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)

    # =============
    set(SRCS_TEST_PROJ
        tests/test_proj.cpp
    )
    add_executable(ProjTest ${SRCS_TEST_PROJ})
    target_link_libraries(ProjTest ${LIBS} ${PROJECT_NAME})
    # ==============

    # =============
    set(SRCS_SAMPLE_WINDOW
        tests/sample_window.cpp
    )
    add_executable(SampleWindow ${SRCS_SAMPLE_WINDOW})
    target_link_libraries(SampleWindow ${LIBS} ${PROJECT_NAME})
    # ==============


endif()