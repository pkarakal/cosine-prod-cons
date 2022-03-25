#.rst:
# FindThreads
# -------------
# Finds pthread library and sets up the compilation and linking flags
#
# This will define the following variable:
#
# THREADS_FOUND - A boolean that checks if pthread library is found


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
set(THREADS_FOUND ${Threads_FOUND})
mark_as_advanced(THREADS_FOUND)
