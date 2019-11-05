add_library( EthercatManager 

    ethercat_manager.cpp
    ecat_dc.c 
)

target_link_libraries( EthercatManager
    ${LIB_XENOMAI}
)