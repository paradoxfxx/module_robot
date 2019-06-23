#########################################################################
# CMakeLists.txt file for building                      #
#                                                                       #
# This file is free software: you can redistribute it and/or modify     #
# it under the terms of the GNU General Public License as published by  #
# the Free Software Foundation, either version 3 of the License, or     #
# (at your option) any later version.                                   #
#                                                                       #
# This program is distributed in the hope that it will be useful,       #
# but WITHOUT ANY WARRANTY; without even the implied warranty of        #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
# GNU General Public License for more details.                          #
#                                                                       #
# You should have received a copy of the GNU General Public License     #
# along with this program.  If not, see <http://www.gnu.org/licenses/>. #
#########################################################################

cmake_minimum_required (VERSION 2.8.12)

project(modular_robot)

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

###############################################################


set(EXAMPLES rtprint trivial-periodic)

set(LIB_XENOMAI
    ${Xenomai_NATIVE_DL_FLAGS}
    ${soem_LIBRARIES}
    ${Boost_LIBRARIES}
)

############### source and test ######################
include_directories( ${PROJECT_SOURCE_DIR}/include )
add_subdirectory( src )
######################################################



################xenomai test file#############################################
add_executable(rtprint rtprint.c)
target_include_directories(rtprint PUBLIC ${Xenomai_NATIVE_INCLUDE_DIRS})
target_link_libraries(rtprint ${Xenomai_NATIVE_DL_FLAGS})
set_target_properties(rtprint PROPERTIES COMPILE_FLAGS ${Xenomai_NATIVE_DEFINITIONS})


add_executable(trivial-periodic trivial-periodic.c)
target_include_directories(trivial-periodic PUBLIC ${Xenomai_NATIVE_INCLUDE_DIRS})
target_link_libraries(trivial-periodic ${Xenomai_NATIVE_DL_FLAGS})
set_target_properties(trivial-periodic PROPERTIES COMPILE_FLAGS ${Xenomai_NATIVE_DEFINITIONS})
################################################################################

