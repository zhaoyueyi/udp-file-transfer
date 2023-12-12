cmake_minimum_required(VERSION 3.27)

set(VCPKG_ROOT "D:/Repository/vcpkg")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic" CACHE STRING "" FORCE)
set(cJSON_DIR "D:\\Repository\\vcpkg\\installed\\x64-mingw-static\\share\\cjson")

project(sample C)

set(CMAKE_C_STANDARD 11)

find_package(libuv CONFIG REQUIRED)
find_package(cJSON CONFIG REQUIRED)

configure_file(config.json ${CMAKE_CURRENT_BINARY_DIR}/config.json COPYONLY)

add_executable(client client/main.c
        config.c
        config.h)
add_executable(server server/main.c
        config.c
        config.h)

target_link_libraries(client uv cjson)
target_link_libraries(server uv cjson)