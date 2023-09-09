cmake_minimum_required(VERSION 3.6)
project(sfm)

find_package(COLMAP REQUIRED mvs)
include_directories(${COLMAP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})
link_directories(${COLMAP_LINK_DIRS})

if (${CERES_VERSION} VERSION_LESS "2.0.0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(${CERES_VERSION} VERSION_LESS "2.2.0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()
set(CMAKE_SKIP_BUILD_RPATH FALSE)

if (${COLMAP_CUDA_ENABLED})
    message("Compiling bindings with CUDA support.")
    add_definitions(-DCUDA_ENABLED)
else()
    message("Compiling bindings without CUDA support.")
endif()

if (${COLMAP_CGAL_ENABLED})
    message("Compiling bindings with CGAL support.")
    add_definitions(-DCGAL_ENABLED)
else()
    message("Compiling bindings without CGAL support.")
endif()

find_package(Boost REQUIRED graph)
find_package(Eigen3 REQUIRED)

include_directories(${COLMAP_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR})
link_directories(${COLMAP_LINK_DIRS})

add_executable(sfm "main.cc")
target_link_libraries(sfm PUBLIC colmap ${COLMAP_LIBRARIES} colmap_mvs colmap_controllers)