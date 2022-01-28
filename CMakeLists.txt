cmake_minimum_required(VERSION 3.8)


# Project
project("lizy1.type" VERSION 0)


# Package
set(NAME_PACK          "${PROJECT_NAME}")
set(NAME_PACK_WITH_VER "${PROJECT_NAME}-${PROJECT_VERSION}")
set(DIR_DATA_INST      "${NAME_PACK}/${NAME_PACK_WITH_VER}")               # data dir (install)


# Target
set(NAME_TARG_MAIN "${PROJECT_NAME}")
add_subdirectory("${NAME_TARG_MAIN}")
add_subdirectory("test_and_example")


# Install Find Package Support
set(DIR_FIND_PACK_INST "${NAME_PACK}/lib/${NAME_PACK_WITH_VER}")           # find-package dir (install)
include(CMakePackageConfigHelpers)
configure_package_config_file(                                             # generate a -config.cmake
    "${CMAKE_SOURCE_DIR}/config.cmake.in"                                  # path config.cmake in
    "${CMAKE_BINARY_DIR}/${NAME_PACK}-config.cmake"                        # path config.cmake out
    INSTALL_DESTINATION "${DIR_FIND_PACK_INST}/${NAME_PACK}-config.cmake"  # path config.cmake install
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
write_basic_package_version_file(                                          # generate the version file for the config file
    "${CMAKE_BINARY_DIR}/${NAME_PACK}-config-version.cmake"                # path config-version.cmake out
    COMPATIBILITY AnyNewerVersion                                          # package compatibility mode
)
install(                                                                   # install -config.cmake & -config-version.cmake
    FILES
        "${CMAKE_BINARY_DIR}/${NAME_PACK}-config.cmake"
        "${CMAKE_BINARY_DIR}/${NAME_PACK}-config-version.cmake"
    DESTINATION "${DIR_FIND_PACK_INST}"
)
