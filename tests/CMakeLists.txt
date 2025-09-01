
# =======================================================
set(DATASETS_DIR ${CMAKE_CURRENT_LIST_DIR}/datasets)
configure_file(./TestDefines.h.in TestDefines.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
# ========================================================

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

set(SRCS_TEST_GEOJSON
    ./test_geojson.cpp
)
add_executable(GeojsonTest ${SRCS_TEST_GEOJSON})
target_link_libraries(GeojsonTest mgis Qt6::Core Qt6::Widgets)

# ============================

set(SRCS_GEOJSON_VIEWER
    ./geojson_viewer.cpp
)
add_executable(GeojsonViewer ${SRCS_GEOJSON_VIEWER})
target_link_libraries(GeojsonViewer mgis Qt6::Core Qt6::Widgets)

# ============================

set(SRCS_STYLE_PROVIDER
    ./test_style_provider.cpp
)
add_executable(StyleProviderTest ${SRCS_STYLE_PROVIDER})
target_link_libraries(StyleProviderTest mgis Qt6::Core Qt6::Widgets)

# ============================

set(SRCS_STYLESET_PROVIDER
    ./test_style_set_provider.cpp
)
add_executable(StyleSetProviderTest ${SRCS_STYLESET_PROVIDER})
target_link_libraries(StyleSetProviderTest mgis Qt6::Core Qt6::Widgets)

# ============================

set(SRCS_JSON_STYLE
    ./test_json_style.cpp
)
add_executable(JsonStyleTest ${SRCS_JSON_STYLE})
target_link_libraries(JsonStyleTest mgis Qt6::Core Qt6::Widgets)