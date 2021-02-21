cmake_minimum_required( VERSION 3.10 )

project( liblhmodeldbutil )

# general flags for building successfully
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

# for debugging
set( LLHC_CXX_FLAGS_DBG "${LLHC_CXX_FLAGS_DBG} -Wall -Wextra -W" )
set( LLHC_CXX_FLAGS_DBG "${LLHC_CXX_FLAGS_DBG} -DDEBUG" )
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${LLHC_CXX_FLAGS_DBG}" )
set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${LLHC_CXX_FLAGS_DBG}" )

##############################################################
# library
##############################################################

set( LIB_SRC_DIR "src" )
set( LIB_INC_DIR "include" )

# pull in gtest
find_package( GTest REQUIRED )
if( NOT GTEST_FOUND )
    message( FATAL_ERROR "gtest could not be found" )
endif( NOT GTEST_FOUND )

add_library( lhmodeldbutil SHARED
    "${LIB_SRC_DIR}/lhmodel_cppdb.cxx" )

target_link_libraries( lhmodeldbutil
    PUBLIC
        lhmodel
        lhmodelutil
        cppdb )

target_include_directories( lhmodeldbutil
    PUBLIC
        "${LIB_INC_DIR}/lhmodeldbutil"
    PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}" )

##############################################################
# unit tests
##############################################################

set( TST_SRC_DIR "./test" )

enable_testing()

add_executable( testlhmodeldbutil
    "${TST_SRC_DIR}/examplemodela.cxx"
    "${TST_SRC_DIR}/testlhmodeldbutil.cxx" )

target_link_libraries( testlhmodeldbutil
    PRIVATE
        "${GTEST_BOTH_LIBRARIES}"
        gmock
        pthread
        lhmodeldbtest
        lhmodeldbutil )

target_include_directories( testlhmodeldbutil
    PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}"
        "${GTEST_INCLUDE_DIRS}" )

include( GoogleTest )
gtest_add_tests( TARGET testlhmodeldbutil )

##############################################################
# installation
##############################################################

# library
install( TARGETS lhmodeldbutil
         LIBRARY
            DESTINATION lib
            COMPONENT lhmodeldbutil )

# include directory
install( DIRECTORY "${LIB_INC_DIR}/lhmodeldbutil/lhmodeldbutil"
         DESTINATION include
         COMPONENT lhmodeldbutil
         FILES_MATCHING PATTERN "*.h" )

ADD_CUSTOM_TARGET( install-lhmodeldbutil
                   ${CMAKE_COMMAND}
                   -D "CMAKE_INSTALL_COMPONENT=lhmodeldbutil"
                   -P "cmake_install.cmake" )
