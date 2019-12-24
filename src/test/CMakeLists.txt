
add_subdirectory(xenomai_test)


######################test file#################################

add_executable(test_response test_response.cpp)
target_link_libraries(test_response ElmoClient EthercatManager )

add_executable(robot_model_test robot_model_test.cpp)
target_link_libraries(robot_model_test robotmodel )

add_executable(joint_impedance_control_test joint_impedance_control_test.cpp)
target_link_libraries(joint_impedance_control_test Controller)


add_executable(pid_test pid_test.cpp)
target_link_libraries(pid_test Controller)


add_executable(pos_test pos_test.cpp)
target_link_libraries(pos_test RobotJointClient)

add_executable(pos_test_1 pos_test_1.cpp)
target_link_libraries(pos_test_1 RobotJointClient)

add_executable(csp_test csp_test.cpp)
target_link_libraries(csp_test RobotJointClient)

add_executable(csv_test csv_test.cpp)
target_link_libraries(csv_test RobotJointClient)

add_executable(cst_test cst_test.cpp)
target_link_libraries(cst_test RobotJointClient)

add_executable(pp_test pp_test.cpp)
target_link_libraries(pp_test RobotJointClient)

add_executable(pv_test pv_test.cpp)
target_link_libraries(pv_test RobotJointClient)

add_executable(pt_test pt_test.cpp)
target_link_libraries(pt_test RobotJointClient)

add_executable(halt_test halt_test.cpp)
target_link_libraries(halt_test RobotJointClient)

add_executable(quick_stop_test quick_stop_test.cpp)
target_link_libraries(quick_stop_test RobotJointClient)

add_executable(read_chip_temp read_chip_temp.cpp)
target_link_libraries(read_chip_temp RobotJointClient)
################################################################
