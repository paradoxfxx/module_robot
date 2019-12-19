# ################xenomai test file#############################################
add_executable(rtprint rtprint.c)
target_include_directories(rtprint PUBLIC ${Xenomai_NATIVE_INCLUDE_DIRS})
target_link_libraries(rtprint ${Xenomai_NATIVE_DL_FLAGS})
set_target_properties(rtprint PROPERTIES COMPILE_FLAGS ${Xenomai_NATIVE_DEFINITIONS})


add_executable(trivial-periodic trivial-periodic.cpp)
target_include_directories(trivial-periodic PUBLIC ${Xenomai_NATIVE_INCLUDE_DIRS})
target_link_libraries(trivial-periodic ${Xenomai_NATIVE_DL_FLAGS})
set_target_properties(trivial-periodic PROPERTIES COMPILE_FLAGS ${Xenomai_NATIVE_DEFINITIONS})



# ################################################################################