cmake_minimum_required( VERSION 3.10 )

project( liblhmodeldbtest )

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

add_library( lhmodeldbtest SHARED
    "${LIB_SRC_DIR}/mock_cppdb_backend.cxx" )

target_link_libraries( lhmodeldbtest
    PUBLIC
        "${GTEST_BOTH_LIBRARIES}"
        gmock
        pthread
        cppdb )

target_include_directories( lhmodeldbtest
    PUBLIC
        "${LIB_INC_DIR}/lhmodeldbtest"
    PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}" )

##############################################################
# installation
##############################################################

# library
install( TARGETS lhmodeldbtest
         LIBRARY
            DESTINATION lib
            COMPONENT lhmodeldbtest )

# include directory
install( DIRECTORY "${LIB_INC_DIR}/lhmodeldbtest/lhmodeldbtest"
         DESTINATION include
         COMPONENT lhmodeldbtest
         FILES_MATCHING PATTERN "*.h" )

ADD_CUSTOM_TARGET( install-lhmodeldbtest
                   ${CMAKE_COMMAND}
                   -D "CMAKE_INSTALL_COMPONENT=lhmodeldbtest"
                   -P "cmake_install.cmake" )
