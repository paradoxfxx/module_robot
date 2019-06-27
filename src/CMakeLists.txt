find_package(orocos_kdl REQUIRED)
include_directories(${orocos_kdl_INCLUDE_DIRS})
message(STATUS "orocos_kdl include path: ${orocos_kdl_INCLUDE_DIRS}")
message(STATUS "orocos_kdl lib path: ${orocos_kdl_LIBRARIES}")

find_package(kdl_parser REQUIRED)
include_directories("/opt/ros/kinetic/include/")

add_subdirectory(kinematics_dynamics)
add_subdirectory(elmo_control)
add_subdirectory(ethercat_manager)
add_subdirectory(robot_control)
add_subdirectory(robot_controller)

add_subdirectory(test)