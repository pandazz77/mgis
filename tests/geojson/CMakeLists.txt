cmake_minimum_required(VERSION 3.5)

# copy geojson datasets =====

# Find all .json files in the current directory
file(GLOB JSON_FILES "*.json")

# Create the output binary directory if it doesn't exist
set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
if(NOT EXISTS ${OUTPUT_DIR})
    file(MAKE_DIRECTORY ${OUTPUT_DIR})
endif()

foreach(JSON_FILE IN LISTS JSON_FILES)
    get_filename_component(JSON_FILE_NAME ${JSON_FILE} NAME)
    file(COPY ${JSON_FILE} DESTINATION ${OUTPUT_DIR})
endforeach()


# ============================

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