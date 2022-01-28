set(DIR_INCL_BUILD "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(DIR_INCL_INST  "${DIR_DATA_INST}/include")


# Main Target
add_library               ("${NAME_TARG_MAIN}" INTERFACE)
target_compile_features   ("${NAME_TARG_MAIN}" INTERFACE cxx_std_20)
target_include_directories("${NAME_TARG_MAIN}" INTERFACE "$<BUILD_INTERFACE:${DIR_INCL_BUILD}>"
                                                         "$<INSTALL_INTERFACE:${DIR_INCL_INST}>")


# Install
install(DIRECTORY "${DIR_INCL_BUILD}" DESTINATION "${DIR_DATA_INST}")
install(TARGETS "${NAME_TARG_MAIN}" EXPORT "${NAME_TARG_MAIN}-installation")     # install main target, generate export info
install(EXPORT "${NAME_TARG_MAIN}-installation" DESTINATION "${DIR_DATA_INST}")  # install export info
