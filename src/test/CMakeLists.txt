######################test file#################################
add_executable(reset reset.cpp)
target_link_libraries(reset elmoclient ethercatmanager)

add_executable(simple_test_csp simple_test_csp.cpp)
target_link_libraries(simple_test_csp elmoclient ethercatmanager )

add_executable(simple_test_csv simple_test_csv.cpp)
target_link_libraries(simple_test_csv elmoclient ethercatmanager )

add_executable(simple_test_cst simple_test_cst.cpp)
target_link_libraries(simple_test_cst elmoclient ethercatmanager )

add_executable(test_response test_response.cpp)
target_link_libraries(test_response elmoclient ethercatmanager )

################################################################
