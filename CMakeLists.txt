cmake_minimum_required( VERSION 3.10 )

project( lhmodeldbutil
         DESCRIPTION "lhmodeldbutil libraries"
         HOMEPAGE_URL "https://github.com/fbuonaro/lhmodeldbutil"
         LANGUAGES CXX
         VERSION 1.0.0 )

enable_testing()

set(LH_SUBDIRECTORIES liblhmodeldbtest liblhmodeldbutil)

include("modules/lhscriptutil/cmake/lhmacros.cmake")

lh_toplevel_add_subdirectories()

lh_toplevel_add_cpack_package_components()