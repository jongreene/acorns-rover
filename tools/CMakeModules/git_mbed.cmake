if(NOT EXISTS "${PLATFORM_ROOT}/lib/mbed-os/sources")
    execute_process(COMMAND mkdir "${PLATFORM_ROOT}/lib/mbed-os/sources")
endif()

if(NOT EXISTS "${PLATFORM_ROOT}/lib/mbed-os/sources/mbed-os-${MBED_RELEASE_VERISON}")
  execute_process(COMMAND mkdir "${PLATFORM_ROOT}/lib/mbed-os/sources/mbed-os-${MBED_RELEASE_VERISON}")
endif()

set(MBED_SUBMODULE_PATH "${PLATFORM_ROOT}/lib/mbed-os/sources/mbed-os-${MBED_RELEASE_VERISON}")

# submodule update
if(NOT EXISTS "${MBED_SUBMODULE_PATH}/platform")
    execute_process(COMMAND git clone https://github.com/ARMmbed/mbed-os.git mbed-os-${MBED_RELEASE_VERISON}
                       WORKING_DIRECTORY ${PLATFORM_ROOT}/lib/mbed-os/sources/)
    message (STATUS "${BoldGreen}finished updating the submodule ${ColourReset}")

    # checking out the mbed os branch to a particular tag
    execute_process(COMMAND git checkout mbed-os-${MBED_RELEASE_VERISON}
                    WORKING_DIRECTORY ${MBED_SUBMODULE_PATH})

    if(EXISTS "${PLATFORM_ROOT}/tools/patches/mbed/${MBED_RELEASE_VERISON}")
        include(${PLATFORM_ROOT}/tools/patches/mbed/${MBED_RELEASE_VERISON}/patch.cmake)
    endif()

endif()

set(MBED_DIR ${MBED_SUBMODULE_PATH})
