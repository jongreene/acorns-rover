cmake_minimum_required(VERSION 3.13.4)
set(MBED_RELEASE_VERISON 5.8.3)
set(CMAKE_BUILD_TYPE RELEASE)

set(PLATFORM_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(MBED_TOP_DIR "${PLATFORM_ROOT}/lib/mbed-os")

project(semi-autonomous-rover)

link_directories(${MBED_TOP_DIR}/libs/EFM32GG_STK3700)

include(${CMAKE_CURRENT_SOURCE_DIR}/tools/CMakeModules/gcc_arm_toolchain.cmake)
include(${PLATFORM_ROOT}/tools/CMakeModules/application_functions.cmake)
include(${PLATFORM_ROOT}/tools/CMakeModules/git_mbed.cmake)
include(${PLATFORM_ROOT}/tools/CMakeModules/target_EFM32GG990F1024.cmake)
include(${PLATFORM_ROOT}/tools/CMakeModules/mbed_EFM32GG990F1024.cmake)

include_directories(include
        core/command-dispatcher/include
        core/HTU21D
        core/MPL3115A2
        core/ALS31300
        core/LCDDisplay
        ${MBED_TOP_DIR}/libs/EFM32GG_STK3700
        ${MBED_TOP_DIR}/config/EFM32GG_STK3700
)

set(SOURCES
        ${SOURCES}
        src/main.cpp
        src/Commands.cpp
        core/command-dispatcher/src/UARTHandler.cpp
        core/command-dispatcher/src/PlatformInterface.cpp
        core/ALS31300/ALS31300.cpp
        core/command-dispatcher/src/Connector.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

set(CMAKE_CURRENT_BINARY_DIR ${PLATFORM_ROOT}/${CMAKE_BUILD_TYPE}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

#todo link from target specific cmake
target_link_libraries(${PROJECT_NAME} mbed-os-${MBED_RELEASE_VERISON} -T${PLATFORM_ROOT}/lib/mbed-os/sources/mbed-os-${MBED_RELEASE_VERISON}/targets/TARGET_Silicon_Labs/TARGET_EFM32/TARGET_EFM32GG/device/TARGET_1024K/TOOLCHAIN_GCC_ARM/efm32gg.ld)

make_mbed_firmware(${PROJECT_NAME})
make_jlink_script("${PLATFORM_ROOT}/${CMAKE_BUILD_TYPE}" ${PROJECT_NAME} 0)