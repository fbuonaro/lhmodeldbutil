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
set( LH_INSTALL_BINDIR "${CMAKE_INSTALL_BINDIR}" )
set( LH_INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}/liblhmodeldbtest_${PROJECT_VERSION}" )
set( LH_INSTALL_INCDIR "${CMAKE_INSTALL_INCLUDEDIR}/lhmodeldbtest_${PROJECT_VERSION}" )

set( LH_LIB_NAME lhmodeldbtest )

# pull in gtest
find_package( GTest REQUIRED )

# source files
set( LH_LIB_SRC_FILES "src/mock_cppdb_backend.cxx" )

# library dependencies
set( LH_LIB_PUBLIC_LINKLIBS 
     "${GTEST_BOTH_LIBRARIES}" 
     gmock 
     pthread 
     cppdb )
set( LH_LIB_PRIVATE_LINKLIBS )
# header dependencies
set( LH_LIB_PUBLIC_INCLUDES 
     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/lhmodeldbtest>   # build
     $<INSTALL_INTERFACE:${LH_INSTALL_INCDIR}> )                               # export
set( LH_LIB_PRIVATE_INCLUDES 
     "${CMAKE_CURRENT_BINARY_DIR}" )

lh_add_library()

##############################################################
# unit tests
##############################################################

# no tests

##############################################################
# installation
##############################################################

# name of component to install
set(LH_COMPONENT_NAME liblhmodeldbtest )
# directories to install
set(LH_LIB_PUBLIC_INCLUDE_DIRS "include/lhmodeldbtest/lhmodeldbtest")

lh_add_install_library()

##############################################################
# pkg-config
##############################################################
set( PC_INCDIR "${LH_INSTALL_INCDIR}")
set( PC_LIBDIR "${LH_INSTALL_LIBDIR}")
set( PC_PUBREQS )
set( PC_PRIVREQS )
set( PC_CFLAGS )
set( PC_PUBLIBS "-llhmodeldbtest -llhmodel -llhmodelutil -lcppdb" )
set( PC_PRIVLIBS )
set( PC_INSTALL_DIR "${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig" )
set( PC_INSTALL_FILENAME "${PROJECT_NAME}-${CMAKE_PROJECT_VERSION_MAJOR}.pc" )
set( PC_INSTALL_FILE "${PC_INSTALL_DIR}/${PC_INSTALL_FILENAME}" )
set( PC_RELOCATE_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/rpm_relocate_pc.sh" )

lh_add_pkgconfig()

##############################################################
# cmake packaging
##############################################################

# to relocate the pc file prefix to /somewhere if 'rpm --install --prefix /somewhere'
set( CPACK_RPM_LIBLHMODELDBTEST_PACKAGE_REQUIRES_POST "sed" PARENT_SCOPE) 
set( CPACK_RPM_LIBLHMODELDBTEST_POST_INSTALL_SCRIPT_FILE "${PC_RELOCATE_SCRIPT}" PARENT_SCOPE)

set(LH_PACKAGE_DESC "liblhmodeldbtest libraries package")

lh_add_cpack_component()
