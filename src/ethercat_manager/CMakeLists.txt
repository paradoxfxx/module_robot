add_library( ethercatmanager 

    ethercat_manager.cpp
    ecat_dc.c 
)

target_link_libraries( ethercatmanager
    ${LIB_XENOMAI}
)