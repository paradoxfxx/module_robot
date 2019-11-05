add_library(Controller SHARED

descartes_impedance_controller.cpp
joint_impedance_control.cpp
pid_controller.cpp


)

target_link_libraries(Controller

    ${orocos_kdl_LIBRARIES}
)