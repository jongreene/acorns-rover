set(GCC_DIR ${PLATFORM_DIR_PATH}/tools/arm-gcc-toolchain)
set(CMAKE_TOOLCHAIN_FILE ./cmake/x86_64.cmake)

# default name for linux and mac
set(GCC_NAME gcc-arm-none-eabi-7-2017-q4-major)

# default extension for linux and mac
set(GCC_NAME_EXTENSION ".tar.bz2")

if(NOT(EXISTS ${GCC_DIR}))
    file(MAKE_DIRECTORY ${GCC_DIR})
endif()

set(GCC_URL "https://developer.arm.com/-/media/Files/downloads/gnu-rm/7-2017q4/gcc-arm-none-eabi-7-2017-q4-major-linux.tar.bz2?revision=375265d4-e9b5-41c8-bf23-56cbe927e156?product=GNU%20Arm%20Embedded%20Toolchain,64-bit,,Linux,7-2017-q4-major")
if(NOT EXISTS "${GCC_DIR}/${GCC_NAME}")
    message("Downloading: ${GCC_NAME}${GCC_NAME_EXTENSION}")
#    file(DOWNLOAD ${GCC_URL} "${GCC_DIR}/${GCC_NAME}${GCC_NAME_EXTENSION}" SHOW_PROGRESS)

    file(DOWNLOAD
            ${GCC_URL}
            "${GCC_DIR}/${GCC_NAME}${GCC_NAME_EXTENSION}"
            SHOW_PROGRESS)

    message("extracting: ${GCC_NAME}${GCC_NAME_EXTENSION}")
    execute_process(
            COMMAND ${CMAKE_COMMAND} -E tar jxvf "${GCC_DIR}/${GCC_NAME}${GCC_NAME_EXTENSION}" # path to file
            WORKING_DIRECTORY ${GCC_DIR}/
    )

    # remove compressed file
    message("Removing: ${GCC_NAME}${GCC_NAME_EXTENSION} archive.")
    file(REMOVE "${GCC_DIR}/${GCC_NAME}${GCC_NAME_EXTENSION}")
endif()

# set GCC_DIR for gcc_arm_toolchain.cmake to use for compiler
set(GCC_DIR "${GCC_DIR}/${GCC_NAME}")

include(CMakeForceCompiler)
CMAKE_FORCE_C_COMPILER(${GCC_DIR}/bin/arm-none-eabi-gcc GNU)
CMAKE_FORCE_CXX_COMPILER(${GCC_DIR}/bin/arm-none-eabi-g++ GNU)