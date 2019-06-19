message("target_EFM32GG990F1024.cmake called\n")

# Set Mbed defines
set(MBED_DEFINES "\
        -DARM_MATH_CM3 \
        -D__CMSIS_RTOS \
        -D__CORTEX_M3 \
        -DDEVICE_ANALOGIN=1 \
        -DDEVICE_ANALOGOUT=1 \
        -DDEVICE_FLASH=1 \
        -DDEVICE_I2C=1 \
        -DDEVICE_I2C_ASYNCH=1 \
        -DDEVICE_I2CSLAVE=1 \
        -DDEVICE_INTERRUPTIN=1 \
        -DDEVICE_ITM=1 \
        -DDEVICE_LOWPOWERTIMER=1 \
        -DDEVICE_PORTIN=1 \
        -DDEVICE_PORTINOUT=1 \
        -DDEVICE_PORTOUT=1 \
        -DDEVICE_PWMOUT=1 \
        -DDEVICE_RTC=1 \
        -DDEVICE_SERIAL=1 \
        -DDEVICE_SERIAL_ASYNCH=1 \
        -DDEVICE_SPI=1 \
        -DDEVICE_SPI_ASYNCH=1 \
        -DDEVICE_SPISLAVE=1 \
        -DDEVICE_STDIO_MESSAGES=1 \
        -DEFM32GG990F1024 \
        -D__MBED__=1 \
        -D__MBED_CMSIS_RTOS_CM \
        -DMBEDTLS_CONFIG_HW_SUPPORT \
        -DTARGET_1024K \
        -DTARGET_CORTEX \
        -DTARGET_CORTEX_M \
        -DTARGET_EFM32 \
        -DTARGET_EFM32GG \
        -DTARGET_LIKE_CORTEX_M3 \
        -DTARGET_EFM32GG990F1024 \
        -DTARGET_EFM32GG_STK3700 \
        -DTARGET_LIKE_MBED \
        -DTARGET_M3 \
        -DTARGET_Silicon_Labs \
        -DTARGET_UVISOR_UNSUPPORTED \
        -DUVISOR=0 \
        -DTOOLCHAIN_GCC \
        -DTOOLCHAIN_GCC_ARM \
        -DTOOLCHAIN_object \
        -DTRANSACTION_QUEUE_SIZE_SPI=4 \
")

#set(MBED_INCLUDES " -include mbed_config.h ")

# Set flags
set(COMMON_FLAGS " -MMD -mcpu=cortex-m3 -mthumb -g3 -O0 -Wno-missing-field-initializers -Wno-unused-parameter -Wall ${MBED_DEFINES} ${MBED_INCLUDES}")

set(CMAKE_CXX_FLAGS " ${COMMON_FLAGS} -std=gnu++14 -fno-rtti -Wvla -include mbed_config.h")

set(CMAKE_C_FLAGS " ${COMMON_FLAGS} -std=gnu14 -include mbed_config.h")

set(CMAKE_EXE_LINKER_FLAGS " -Wl,--gc-sections -Wl,--wrap,main -Wl,-n -mcpu=cortex-m3 -mthumb -T${MBED_DIR}/targets/TARGET_Silicon_Labs/TARGET_EFM32/TARGET_EFM32GG/device/TARGET_1024K/TOOLCHAIN_GCC_ARM/efm32gg.ld -Wl,--no-whole-archive  ")

# Set Include Directories
include_directories(${MBED_INCLUDE_DIR})