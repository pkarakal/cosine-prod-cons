cmake_minimum_required(VERSION 3.10)
project(cosine_prod_cons LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

option(ENABLE_CLANGTIDY "Enable clang-tidy support?" OFF)
option(ENABLE_BOOST "Enable Boost support" ON)

include(cmake/FindThreads.cmake)



if (ENABLE_CLANGTIDY)
    include(cmake/FindClangTidy.cmake)
endif ()

if(ENABLE_BOOST)
    include(cmake/FindBoost.cmake)
    if(BOOST_FOUND)
        link_libraries(${BOOST_LIBRARIES})
        set(USE_BOOST 1)
    endif()
endif()


configure_file(lib/definitions.h.in lib/definitions.h)

add_library(cosine_prod_cons_lib OBJECT lib/cosine_prod_cons_lib.h lib/cosine_prod_cons_lib.cpp)

add_executable(cosine_prod_cons cosine-prod-cons.cpp)
target_link_libraries(cosine_prod_cons cosine_prod_cons_lib)

if(THREADS_FOUND)
    target_link_libraries(cosine_prod_cons Threads::Threads)
else()
    message(FATAL_ERROR "Pthread not found. Please install pthread library." )
endif()
