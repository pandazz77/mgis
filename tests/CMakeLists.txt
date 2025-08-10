
set(SRCS_TEST_PROJ
    ./test_proj.cpp
)
add_executable(ProjTest ${SRCS_TEST_PROJ})
target_link_libraries(ProjTest mgis Qt6::Core Qt6::Widgets)

# =======================================================

set(SRCS_SAMPLE_WINDOW
    ./sample_window.cpp
)
add_executable(SampleWindow ${SRCS_SAMPLE_WINDOW})
target_link_libraries(SampleWindow mgis Qt6::Core Qt6::Widgets)


# =======================================================

set(SRCS_TEST_GEOJSON
    ./test_geojson.cpp
)
add_executable(GeojsonTest ${SRCS_TEST_GEOJSON})
target_link_libraries(GeojsonTest mgis Qt6::Core Qt6::Widgets)