cmake_minimum_required( VERSION 3.10 )

project( liblhmodeldbtest 
         DESCRIPTION "lhmodeldbtest library"
         HOMEPAGE_URL "https://github.com/fbuonaro/lhmodeldbtest/liblhmodeldbtest"
         LANGUAGES CXX
         VERSION 1.0.0 )

##############################################################
# general
##############################################################

include( GNUInstallDirs )

set( CMAKE_CXX_STANDARD 11 )

add_compile_options( -Wall -Wextra -pedantic -Werror )

##############################################################
# library
##############################################################

# install directories
set( INSTALL_BINDIR "${CMAKE_INSTALL_BINDIR}" )
set( INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}/liblhmodeldbtest_${PROJECT_VERSION}" )
set( INSTALL_INCDIR "${CMAKE_INSTALL_INCLUDEDIR}/lhmodeldbtest_${PROJECT_VERSION}" )

# pull in gtest
find_package( GTest REQUIRED )

# source files
set( LIB_SRC_FILES "src/mock_cppdb_backend.cxx" )

# library dependencies
set( LIB_PUBLIC_LINKLIBS 
     "${GTEST_BOTH_LIBRARIES}" 
     gmock 
     pthread 
     cppdb )
set( LIB_PRIVATE_LINKLIBS )
# header dependencies
set( LIB_PUBLIC_INCLUDES 
     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/lhmodeldbtest>   # build
     $<INSTALL_INTERFACE:${INSTALL_INCDIR}> )                               # export
set( LIB_PRIVATE_INCLUDES 
     "${CMAKE_CURRENT_BINARY_DIR}" )

# boilerplate ..
# libs
add_library( lhmodeldbtest SHARED ${LIB_SRC_FILES} )
add_library( lhmodeldbtest_static STATIC ${LIB_SRC_FILES} )
# lib deps
target_link_libraries( lhmodeldbtest
                       PUBLIC "${LIB_PUBLIC_LINKLIBS}"
                       PRIVATE "${LIB_PRIVATE_LINKLIBS}" )
target_link_libraries( lhmodeldbtest_static
                       PUBLIC "${LIB_PUBLIC_LINKLIBS}"
                       PRIVATE "${LIB_PRIVATE_LINKLIBS}" )
# header deps
target_include_directories( lhmodeldbtest
                            PUBLIC "${LIB_PUBLIC_INCLUDES}"
                            PRIVATE "${LIB_PRIVATE_INCLUDES}" )
target_include_directories( lhmodeldbtest_static
                            PUBLIC "${LIB_PUBLIC_INCLUDES}"
                            PRIVATE "${LIB_PRIVATE_INCLUDES}" )
# properties
set_target_properties( lhmodeldbtest
                       PROPERTIES 
                           LIBRARY_OUTPUT_DIRECTORY "${INSTALL_LIBDIR}"
                           SOVERSION ${PROJECT_VERSION}
                           VERSION ${PROJECT_VERSION}
                           CLEAN_DIRECT_OUTPUT 1)
set_target_properties( lhmodeldbtest_static 
                       PROPERTIES
                           ARCHIVE_OUTPUT_DIRECTORY "${INSTALL_LIBDIR}"
                           OUTPUT_NAME lhmodeldbtest )
# .. boilerplate

##############################################################
# unit tests
##############################################################

# no tests

##############################################################
# installation
##############################################################

# boilerplate ..
# install lib/bin
install( TARGETS lhmodeldbtest lhmodeldbtest_static
         EXPORT liblhmodeldbtest-targets 
         COMPONENT liblhmodeldbtest
         RUNTIME DESTINATION "${INSTALL_BINDIR}"
         COMPONENT liblhmodeldbtest
         LIBRARY DESTINATION "${INSTALL_LIBDIR}"
         COMPONENT liblhmodeldbtest
         ARCHIVE DESTINATION "${INSTALL_LIBDIR}"
         COMPONENT liblhmodeldbtest
         INCLUDES DESTINATION "${LIB_PUBLIC_INCLUDES}" )
# install header dir
install( DIRECTORY "include/lhmodeldbtest/lhmodeldbtest"
         DESTINATION "${INSTALL_INCDIR}"
         COMPONENT liblhmodeldbtest
         FILES_MATCHING PATTERN "*.h" )
# install export for things outside of build
install( EXPORT liblhmodeldbtest-targets 
         FILE liblhmodeldbtestTargets.cmake 
         NAMESPACE liblhmodeldbtest:: 
         DESTINATION "${INSTALL_LIBDIR}/cmake" 
         COMPONENT liblhmodeldbtest )
# export export for things in build
export( EXPORT liblhmodeldbtest-targets
    FILE ${CMAKE_BINARY_DIR}/cmake/liblhmodeldbtestTargets.cmake 
    NAMESPACE liblhmodeldbtest:: )
ADD_CUSTOM_TARGET( install-lhmodeldbtest
                   ${CMAKE_COMMAND}
                   -D "CMAKE_INSTALL_COMPONENT=liblhmodeldbtest"
                   -P "cmake_install.cmake" )
# .. boilerplate

##############################################################
# pkg-config
##############################################################
set( PC_INCDIR "${INSTALL_INCDIR}")
set( PC_LIBDIR "${INSTALL_LIBDIR}")
set( PC_PUBREQS )
set( PC_PRIVREQS )
set( PC_CFLAGS )
set( PC_PUBLIBS "-llhmodeldbtest -llhmodel -llhmodelutil -lcppdb" )
set( PC_PRIVLIBS )
set( PC_INSTALL_DIR "${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig" )
set( PC_INSTALL_FILENAME "${PROJECT_NAME}-${CMAKE_PROJECT_VERSION_MAJOR}.pc" )
set( PC_INSTALL_FILE "${PC_INSTALL_DIR}/${PC_INSTALL_FILENAME}" )
set( PC_RELOCATE_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/rpm_relocate_pc.sh" )

# boilerplate ..
# configure pc file
configure_file( "${CMAKE_SOURCE_DIR}/modules/lhscriptutil/cmake/pkgconfig/component.pc.in"
                "${CMAKE_CURRENT_BINARY_DIR}/${PC_INSTALL_FILENAME}"
                @ONLY )
# install pc file
install( FILES "${CMAKE_CURRENT_BINARY_DIR}/${PC_INSTALL_FILENAME}"
         DESTINATION "${PC_INSTALL_DIR}"
         COMPONENT liblhmodeldbtest )
# configure pc relocation script
configure_file( "${CMAKE_SOURCE_DIR}/modules/lhscriptutil/cmake/pkgconfig/rpm_relocate_pc.sh.in"
                "${CMAKE_CURRENT_BINARY_DIR}/rpm_relocate_pc.sh"
                @ONLY )
# .. boilerplate

##############################################################
# cmake packaging
##############################################################

# boilerplate ..
include(CMakePackageConfigHelpers)

configure_package_config_file( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
                               "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                               INSTALL_DESTINATION "${INSTALL_LIBDIR}/cmake" )

write_basic_package_version_file( "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
                                  VERSION "${PROJECT_VERSION}"
                                  COMPATIBILITY SameMajorVersion )

install( FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
         DESTINATION "${INSTALL_LIBDIR}/cmake"
         COMPONENT liblhmodeldbtest )
# .. boilerplate

##############################################################
# rpm packaging
##############################################################
 
# boilerplate ..
set( CPACK_PROJECT_NAME ${PROJECT_NAME} )
set( CPACK_PROJECT_VERSION ${PROJECT_VERSION} )
set( CPACK_COMPONENTS_GROUPING ONE_PER_GROUP )
set( CPACK_GENERATOR "RPM" )

# set( CPACK_GENERATOR "RPM" "TGZ" )
# CPACK_RPM_BUILD_REQUIRES
# CPACK_RPM_liblhmodeldbtest_REQUIRES # component requires

set( CPACK_RPM_PACKAGE_RELOCATABLE ON)
set( CPACK_RPM_COMPONENT_INSTALL ON )

# to relocate the pc file prefix to /somewhere if 'rpm --install --prefix /somewhere'
set( CPACK_RPM_PACKAGE_REQUIRES_POST "sed" ) 
set( CPACK_RPM_POST_INSTALL_SCRIPT_FILE "${PC_RELOCATE_SCRIPT}" )

include( CPack )

# component, group can be different; group will appended to package
cpack_add_component(liblhmodeldbtest
    DISPLAY_NAME  "lhmodeldbtest library"
    DESCRIPTION   "lhmodeldbtest library"
    GROUP liblhmodeldbtest)
cpack_add_component_group(liblhmodeldbtest)
# .. boilerplate