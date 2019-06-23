add_library( ethercatmanager 
    ethercat_manager/ethercat_manager.cpp
    ethercat_manager/ecat_dc.c 
)

target_link_libraries( ethercatmanager
    ${LIB_XENOMAI}
)

add_library(elmoclient 
    elmo_control/elmo_client.cpp
)

target_link_libraries(elmoclient
    ethercatmanager
)

add_library(robotclient 
    robot_control/robot_client.cpp
)
target_link_libraries(robotclient
    ethercatmanager
    elmoclient
)

######################test file#################################
add_executable(reset test/reset.cpp)
target_link_libraries(reset elmoclient ethercatmanager)

add_executable(simple_test_csp test/simple_test_csp.cpp)
target_link_libraries(simple_test_csp elmoclient ethercatmanager )

add_executable(simple_test_csv test/simple_test_csv.cpp)
target_link_libraries(simple_test_csv elmoclient ethercatmanager )

add_executable(simple_test_cst test/simple_test_cst.cpp)
target_link_libraries(simple_test_cst elmoclient ethercatmanager )

add_executable(test_response test/test_response.cpp)
target_link_libraries(test_response elmoclient ethercatmanager )

################################################################

