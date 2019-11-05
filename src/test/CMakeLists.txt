######################test file#################################
add_executable(reset reset.cpp)
target_link_libraries(reset ElmoClient EthercatManager)

add_executable(simple_test_csp simple_test_csp.cpp)
target_link_libraries(simple_test_csp ElmoClient EthercatManager )

add_executable(simple_test_csv simple_test_csv.cpp)
target_link_libraries(simple_test_csv ElmoClient EthercatManager )

add_executable(simple_test_cst simple_test_cst.cpp)
target_link_libraries(simple_test_cst ElmoClient EthercatManager )

# add_executable(test_response test_response.cpp)
# target_link_libraries(test_response ElmoClient EthercatManager )

add_executable(robot_model_test robot_model_test.cpp)
target_link_libraries(robot_model_test robotmodel )

add_executable(joint_impedance_control_test joint_impedance_control_test.cpp)
target_link_libraries(joint_impedance_control_test Controller)


add_executable(pid_test pid_test.cpp)
target_link_libraries(pid_test Controller)
################################################################
