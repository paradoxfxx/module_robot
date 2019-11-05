cmake_minimum_required (VERSION 2.8.12)

add_compile_options(-std=c++11)

project(modular_robot)

#SET(CMAKE_BUILD_TYPE DEBUG)
SET(CMAKE_BUILD_TYPE RELEASE)

set(ENV{XENOMAI_ROOT_DIR} /usr/xenomai)

# Needed Modules
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(FindPkgConfig)
include(${PROJECT_SOURCE_DIR}/cmake-modules/CMakeFunctions.cmake)

# Set policy for LINK_INTERFACE
cmake_policy(SET CMP0022 NEW)
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib )
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake-modules)

############### dependencies ##################################

#common commands for building c++ executables and libraries
find_package(Xenomai)
include_directories( ${Xenomai_NATIVE_INCLUDE_DIRS} )
message(STATUS "xenomai path: ${Xenomai_NATIVE_INCLUDE_DIRS}")
link_directories(
    /usr/xenomai/lib
)

find_package(soem)
include_directories( ${soem_INCLUDE_DIRS} )
message(STATUS "soem path: ${soem_INCLUDE_DIRS}")

find_package(Boost REQUIRED COMPONENTS regex system thread)
include_directories( ${Boost_INCLUDE_DIRS} )
message(STATUS "boost include path: ${Boost_INCLUDE_DIRS}")
message(STATUS "boost lib path: ${Boost_LIBRARY_DIRS}")
link_directories(${Boost_LIBRARY_DIR})

find_package(Eigen3 REQUIRED)
INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})
message(STATUS "eigen3 include path: ${EIGEN3_INCLUDE_DIR}")
###############################################################


# set(EXAMPLES rtprint trivial-periodic)

set(LIB_XENOMAI 
    ${Xenomai_NATIVE_DL_FLAGS}
    ${soem_LIBRARIES}
    ${Boost_LIBRARIES}
)

############### source and test ######################
include_directories( ${PROJECT_SOURCE_DIR}/include )
add_subdirectory( src )
######################################################