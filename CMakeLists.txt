cmake_minimum_required(VERSION 3.12)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tools/CMakeModules/gcc_arm_toolchain.cmake")
set(MBED_RELEASE_VERISON 5.8.3)
set(PLATFORM_DIR_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(MBED_TOP_DIR "${PLATFORM_DIR_PATH}/lib/mbed-os")

project(remote_control_car)

link_directories(${PLATFORM_DIR_PATH}/lib/mbed-os/libs/EFM32GG_STK3700)

include(${PLATFORM_DIR_PATH}/tools/CMakeModules/application_functions.cmake)
include(${PLATFORM_DIR_PATH}/tools/CMakeModules/git_mbed.cmake)
include(${PLATFORM_DIR_PATH}/tools/CMakeModules/target_EFM32GG990F1024.cmake)
include(${PLATFORM_DIR_PATH}/tools/CMakeModules/mbed_EFM32GG990F1024.cmake)
include(${PLATFORM_DIR_PATH}/core/HTU21D/HTU21D.cmake)
include(${PLATFORM_DIR_PATH}/core/MPL3115A2/MPL3115A2.cmake)
include(${PLATFORM_DIR_PATH}/core/ALS31300/ALS31300.cmake)


include_directories(
        include
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
        src/UARTHandler.cpp
        core/command-dispatcher/src/PlatformInterface.cpp
        )

add_executable(${PROJECT_NAME} ${SOURCES})

set(CMAKE_CURRENT_BINARY_DIR ${PLATFORM_DIR_PATH}/release/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} mbed-os-${MBED_RELEASE_VERISON} -T${PLATFORM_DIR_PATH}/lib/mbed-os/sources/mbed-os-${MBED_RELEASE_VERISON}/targets/TARGET_Silicon_Labs/TARGET_EFM32/TARGET_EFM32GG/device/TARGET_1024K/TOOLCHAIN_GCC_ARM/efm32gg.ld)
make_mbed_firmware(${PROJECT_NAME})
#function(make_jlink_script OUTPUT_DIR PROJECT_NAME MBED_APP_START_ADDRESS)
make_jlink_script(${PLATFORM_DIR_PATH}/release ${PROJECT_NAME} 0)
#function(show_memory_map PROJECT_TARGET SHOW_MEMORY_MAP PROJECT_SOURCE_DIR PROJECT_NAME)