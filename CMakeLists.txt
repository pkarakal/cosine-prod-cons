cmake_minimum_required(VERSION 3.10)
project(cosine_prod_cons)

set(CMAKE_CXX_STANDARD 14)

option(ENABLE_CLANGTIDY "Enable clang-tidy support?" OFF)

if (ENABLE_CLANGTIDY)
    include(cmake/FindClangTidy.cmake)
endif ()


add_executable(cosine_prod_cons main.cpp)
