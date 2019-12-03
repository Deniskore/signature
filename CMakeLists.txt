cmake_minimum_required(VERSION 3.14)
project(signature)
find_package(GTest CONFIG REQUIRED)

option(ENABLE_ADDRESS_SANITIZER "Debug with ADDRESS SANITIZER" OFF)

if (ENABLE_ADDRESS_SANITIZER)
    message(STATUS "ADDRESS SANITIZER enabled")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O1 -fno-omit-frame-pointer -fsanitize=address")
    set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -O1 -fno-omit-frame-pointer -fsanitize=address")
endif()

if(UNIX)
    find_package(Threads REQUIRED)
    set(ADDITIONAL_LIBRARIES Threads::Threads)
else()
    set(ADDITIONAL_LIBRARIES "")
endif()

# Remove MBEDTLS_VS2010 PROJECTS
if(MSVC)
    file (REMOVE_RECURSE mbedtls/visualc)
endif()

set(ENABLE_PROGRAMS OFF CACHE INTERNAL "Disable mbedtls programs")
set(ENABLE_TESTING OFF CACHE INTERNAL "Disable mbedtls testing")

add_subdirectory(mbedtls)

include_directories(mbedtls/include
                    mbedtls/crypto/include/mbedtls
                    )

add_custom_target(copy-files ALL
    COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/test_files ${CMAKE_BINARY_DIR}/test_files
    DEPENDS signature)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(signature main.cpp hashcalc.cpp utils.h hashcalc.h)
target_include_directories(signature PUBLIC)
target_link_libraries(signature mbedtls ${ADDITIONAL_LIBRARIES})

project(tests)
add_executable(tests tests.cpp hashcalc.cpp utils.h hashcalc.h)
add_dependencies(tests copy-files)
target_include_directories(tests PUBLIC ${GTEST_INCLUDE_DIRS})
target_link_libraries(tests PRIVATE mbedtls GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main ${ADDITIONAL_LIBRARIES})
