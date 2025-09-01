
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

set(SRCS_TEST_EXP_PARSER
    ./test_exp_parser.cpp
)
add_executable(ExpParserTest ${SRCS_TEST_EXP_PARSER})
target_link_libraries(ExpParserTest mgis Qt6::Core Qt6::Widgets)

# =======================================================

add_subdirectory(./geojson)