find_library(CRYPT_LIB crypt)

add_executable(Cracker src/cracker.cpp)

target_compile_features(Cracker PRIVATE cxx_auto_type)
target_link_libraries(Cracker "${CRYPT_LIB}")
