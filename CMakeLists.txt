cmake_minimum_required(VERSION 3.5)
project(mgis)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SRCS
    src/Projection/Projection.cpp
    src/Projection/SimpleProjection.cpp
    src/Projection/Mercator.cpp
    src/Projection/SphericalMercator.cpp

    src/Features/IFeature.cpp
    src/Features/Feature.cpp
    src/Features/FeatureCollection.cpp

    src/Stylers/IStyler.cpp
    src/Stylers/PointStyler.cpp
    src/Stylers/LineStyler.cpp
    src/Stylers/PolyStyler.cpp
    src/Stylers/RandomStyler.cpp
    src/Stylers/Stylers.cpp

    src/MapView/MapGraphicsScene.cpp
    src/MapView/MapGraphicsView.cpp
    src/MapView/MapCamera.cpp

    src/MapView/QFeatureConvertor.cpp

    src/MapView/Layers/Base/MapPane.cpp
    src/MapView/Layers/Base/ILayer.cpp
    
    src/MapView/Layers/LayerGroup.cpp

    src/MapView/Layers/Feature/IFeatureLayer.cpp
    src/MapView/Layers/Feature/FeatureLayer.cpp
    src/MapView/Layers/Feature/FeatureCollectionLayer.cpp

    src/Providers/FeatureLayerProvider.cpp
    src/Providers/GeoJsonProvider.cpp

    src/Providers/StyleProvider.cpp
    src/Providers/SimpleStyleProvider.cpp
)

set(HDRS
    include/GeoTypes.hpp

    include/Projection/Projection.h
    include/Projection/SimpleProjection.h
    include/Projection/Mercator.h
    include/Projection/SphericalMercator.h

    include/Features/IFeature.h
    include/Features/Feature.h
    include/Features/FeatureCollection.h
    include/Features/FeatureProperties.hpp

    include/Stylers/IStyler.h
    include/Stylers/PointStyler.h
    include/Stylers/LineStyler.h
    include/Stylers/PolyStyler.h
    include/Stylers/RandomStyler.h
    include/Stylers/Stylers.h

    include/MapView/MapGraphicsScene.h
    include/MapView/MapGraphicsView.h
    include/MapView/MapCamera.h

    include/MapView/QFeatureConvertor.h
    
    include/MapView/Layers/Base/LayerSet.hpp
    include/MapView/Layers/Base/MapPane.h
    include/MapView/Layers/Base/ILayer.h
    include/MapView/Layers/Base/TLayerGroup.hpp

    include/MapView/Layers/LayerGroup.h

    include/MapView/Layers/Feature/IFeatureLayer.h
    include/MapView/Layers/Feature/FeatureLayer.h
    include/MapView/Layers/Feature/FeatureCollectionLayer.h

    include/Providers/LayerProvider.hpp
    include/Providers/FeatureLayerProvider.h
    include/Providers/GeoJsonProvider.h

    include/Providers/StyleProvider.h
    include/Providers/SimpleStyleProvider.h
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

target_include_directories(${PROJECT_NAME} PUBLIC 
    include/
    include/Projection
    include/MapView
    include/MapView/Layers
    include/MapView/Layers/Base
    include/MapView/Layers/Feature
    include/Stylers
    include/Features
    include/Providers
)

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    add_subdirectory(./tests)
endif()