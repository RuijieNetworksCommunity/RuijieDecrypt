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
target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})

# Add Libintl Libraries
find_package(Intl REQUIRED)
target_link_libraries(${PROJECT_NAME} ${Intl_LIBRARY})

# setup gettext
set(GETTEXT_DOMAIN "main")
set(GETTEXT_TARGET "Lang")
set(GETTEXT_OUTPUT_DIR "Lang")
set(GETTEXT_LANGUAGES "en" "zh_CN")

target_compile_definitions(${PROJECT_NAME} PUBLIC "GETTEXT_DOMAIN=\"${GETTEXT_DOMAIN}\"")
target_compile_definitions(${PROJECT_NAME} PUBLIC "GETTEXT_OUTPUT_DIR=\"${GETTEXT_OUTPUT_DIR}\"")

include("GetText.cmake")
CONFIGURE_GETTEXT(
        DOMAIN ${GETTEXT_DOMAIN}
        TARGET_NAME ${GETTEXT_TARGET}
        SOURCES "main.c" "password.c" "crypt.c"
        POTFILE_DESTINATION ${GETTEXT_OUTPUT_DIR}
        XGETTEXT_ARGS
        "--keyword=_"
        "--add-comments=TRANSLATORS:Ashley Lee"
        "--package-name=${PROJECT_NAME}"
        "--package-version=${PROJECT_VERSION}"
        "--copyright-holder=Ashley Lee"
        LANGUAGES ${GETTEXT_LANGUAGES}
        BUILD_DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>/${GETTEXT_OUTPUT_DIR}
        ALL
)