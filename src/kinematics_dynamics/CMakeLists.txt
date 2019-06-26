find_package(orocos_kdl REQUIRED)
include_directories(${orocos_kdl_INCLUDE_DIRS})
message(STATUS "orocos_kdl include path: ${orocos_kdl_INCLUDE_DIRS}")
message(STATUS "orocos_kdl lib path: ${orocos_kdl_LIBRARIES}")


add_library(robotmodel SHARED
    robot_model.cpp
)

target_link_libraries(robotmodel 
    ${orocos_kdl_LIBRARIES}
    ${kdl_parser_LIBRARIES}
)

