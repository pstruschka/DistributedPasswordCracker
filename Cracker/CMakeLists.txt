find_library(CRYPT_LIB crypt)
find_package(Threads)

add_executable(Cracker src/cracker.cpp src/threaded_crpyt.c includes/threaded_crpyt.h includes/crypt_data.h src/crypt_data.cpp)

target_include_directories(Cracker PRIVATE includes)

target_compile_features(Cracker PRIVATE cxx_auto_type)
target_link_libraries(Cracker "${CRYPT_LIB}" "${CMAKE_THREAD_LIBS_INIT}")
