cmake_minimum_required(VERSION 3.28)
project(Ruijie_Toolkit C)
set(CMAKE_C_STANDARD 11)
set(PROJECT_VERSION 1.0.1)

file(GLOB_RECURSE INCLUDES "include/*.h" "include/*.c")
aux_source_directory("./src" SOURCES)

# Output exe
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/output)
set(LIBCRYPTO_PATH $ENV{OPENSSL_ROOT_DIR}/bin/libcrypto-3-x64.dll)

add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDES} app.rc)

# Add OpenSSL Libraries
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
        COMMENT "Copying DLL" VERBATIM
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBCRYPTO_PATH} ${EXECUTABLE_OUTPUT_PATH})