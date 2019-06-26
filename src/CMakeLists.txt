find_package(kdl_parser REQUIRED)
include_directories("/opt/ros/kinetic/include/")

add_subdirectory(kinematics_dynamics)
add_subdirectory(elmo_control)
add_subdirectory(ethercat_manager)
add_subdirectory(robot_control)

add_subdirectory(test)