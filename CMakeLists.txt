cmake_minimum_required(VERSION 3.28)
project(Ruijie_Toolkit C)
set(CMAKE_C_STANDARD 11)
set(PROJECT_VERSION 1.0.0)

add_executable(${PROJECT_NAME} main.c
        password.c
        crypt.c
        app.rc
)

# Add OpenSSL Libraries
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})