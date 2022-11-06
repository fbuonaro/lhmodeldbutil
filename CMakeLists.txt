cmake_minimum_required( VERSION 3.10 )

project( lhmodeldbutil
         DESCRIPTION "lhmodeldbutil libraries"
         HOMEPAGE_URL "https://github.com/fbuonaro/lhmodeldbutil"
         LANGUAGES CXX
         VERSION 1.0.0 )

enable_testing()

add_subdirectory( liblhmodeldbtest )
add_subdirectory( liblhmodeldbutil )
