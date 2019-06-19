## Add Mbed library
set(MBED_LINK_LIBS
			${MBED_BUILD})

set(MBED_DIR "${MBED_TOP_DIR}/sources/mbed-os-${MBED_RELEASE_VERISON}")

# Set Mbed Include directories
set(MBED_INCLUDE_DIR
		${MBED_DIR}
		${MBED_DIR}/cmsis
		${MBED_DIR}/cmsis/TARGET_CORTEX_M
		${MBED_DIR}/drivers
		${MBED_DIR}/events
		${MBED_DIR}/events/equeue
		${MBED_DIR}/features
		${MBED_DIR}/features/FEATURE_UVISOR
		${MBED_DIR}/features/FEATURE_UVISOR/importer
		${MBED_DIR}/features/FEATURE_UVISOR/includes
		${MBED_DIR}/features/FEATURE_UVISOR/includes/uvisor
		${MBED_DIR}/features/FEATURE_UVISOR/includes/uvisor/api
		${MBED_DIR}/features/FEATURE_UVISOR/includes/uvisor/api/inc
		${MBED_DIR}/features/FEATURE_UVISOR/includes/uvisor-lib
		${MBED_DIR}/features/FEATURE_UVISOR/includes/uvisor-lib/rtx
		${MBED_DIR}/features/FEATURE_UVISOR/source
		${MBED_DIR}/features/FEATURE_UVISOR/source/rtx
		${MBED_DIR}/features/FEATURE_UVISOR/targets
		${MBED_DIR}/features/FEATURE_UVISOR/targets/TARGET_UVISOR_SUPPORTED/TARGET_EFM32/TARGET_DEBUG/TARGET_M3
		${MBED_DIR}/features/FEATURE_UVISOR/targets/TARGET_UVISOR_SUPPORTED/TARGET_EFM32/TARGET_RELEASE/TARGET_M3
		${MBED_DIR}/features/FEATURE_UVISOR/targets/TARGET_UVISOR_UNSUPPORTED
		${MBED_DIR}/features/cellular
		${MBED_DIR}/features/cellular/easy_cellular
		${MBED_DIR}/features/cellular/framework
		${MBED_DIR}/features/cellular/framework/API
		${MBED_DIR}/features/cellular/framework/AT
		${MBED_DIR}/features/cellular/framework/common
		${MBED_DIR}/features/cellular/framework/targets
		${MBED_DIR}/features/cellular/framework/targets/QUECTEL
		${MBED_DIR}/features/cellular/framework/targets/QUECTEL/BC95
		${MBED_DIR}/features/cellular/framework/targets/QUECTEL/BG96
		${MBED_DIR}/features/cellular/framework/targets/TELIT
		${MBED_DIR}/features/cellular/framework/targets/TELIT/HE910
		${MBED_DIR}/features/cellular/framework/targets/UBLOX
		${MBED_DIR}/features/cellular/framework/targets/UBLOX/LISA_U
		${MBED_DIR}/features/filesystem
		${MBED_DIR}/features/filesystem/bd
		${MBED_DIR}/features/filesystem/fat
		${MBED_DIR}/features/filesystem/fat/ChaN
		${MBED_DIR}/features/filesystem/littlefs
		${MBED_DIR}/features/filesystem/littlefs/littlefs
		${MBED_DIR}/features/frameworks
		${MBED_DIR}/features/frameworks/greentea-client
		${MBED_DIR}/features/frameworks/greentea-client/greentea-client
		${MBED_DIR}/features/frameworks/greentea-client/source
		${MBED_DIR}/features/frameworks/unity
		${MBED_DIR}/features/frameworks/unity/source
		${MBED_DIR}/features/frameworks/unity/unity
		${MBED_DIR}/features/frameworks/utest
		${MBED_DIR}/features/frameworks/utest/source
		${MBED_DIR}/features/frameworks/utest/utest
		${MBED_DIR}/features/lorawan
		${MBED_DIR}/features/lorawan/lorastack
		${MBED_DIR}/features/lorawan/lorastack/mac
		${MBED_DIR}/features/lorawan/lorastack/phy
		${MBED_DIR}/features/lorawan/system
		${MBED_DIR}/features/mbedtls
		${MBED_DIR}/features/mbedtls/importer
		${MBED_DIR}/features/mbedtls/inc
		${MBED_DIR}/features/mbedtls/inc/mbedtls
		${MBED_DIR}/features/mbedtls/platform
		${MBED_DIR}/features/mbedtls/platform/inc
		${MBED_DIR}/features/mbedtls/platform/src
		${MBED_DIR}/features/mbedtls/src
		${MBED_DIR}/features/mbedtls/targets
		${MBED_DIR}/features/mbedtls/targets/TARGET_Silicon_Labs
		${MBED_DIR}/features/nanostack
		${MBED_DIR}/features/netsocket
		${MBED_DIR}/features/netsocket/cellular
		${MBED_DIR}/features/netsocket/cellular/generic_modem_driver
		${MBED_DIR}/features/nvstore
		${MBED_DIR}/features/nvstore/source
		${MBED_DIR}/features/storage
		${MBED_DIR}/hal
		${MBED_DIR}/hal/storage_abstraction
		${MBED_DIR}/platform
		${MBED_DIR}/rtos
		${MBED_DIR}/rtos/TARGET_CORTEX
		${MBED_DIR}/rtos/TARGET_CORTEX/TARGET_CORTEX_M
		${MBED_DIR}/rtos/TARGET_CORTEX/TARGET_CORTEX_M/TOOLCHAIN_GCC
		${MBED_DIR}/rtos/TARGET_CORTEX/rtx4
		${MBED_DIR}/rtos/TARGET_CORTEX/rtx5
		${MBED_DIR}/rtos/TARGET_CORTEX/rtx5/Include
		${MBED_DIR}/rtos/TARGET_CORTEX/rtx5/RTX
		${MBED_DIR}/rtos/TARGET_CORTEX/rtx5/RTX/Config
		${MBED_DIR}/rtos/TARGET_CORTEX/rtx5/RTX/Include
		${MBED_DIR}/rtos/TARGET_CORTEX/rtx5/RTX/Source
		${MBED_DIR}/rtos/TARGET_CORTEX/rtx5/RTX/Source/TOOLCHAIN_GCC
		${MBED_DIR}/rtos/TARGET_CORTEX/rtx5/RTX/Source/TOOLCHAIN_GCC/TARGET_M3
		${MBED_DIR}/rtos/TARGET_CORTEX/rtx5/Source
		${MBED_DIR}/targets
		${MBED_DIR}/targets/TARGET_Silicon_Labs
		${MBED_DIR}/targets/TARGET_Silicon_Labs/TARGET_EFM32
		${MBED_DIR}/targets/TARGET_Silicon_Labs/TARGET_EFM32/TARGET_EFM32GG
		${MBED_DIR}/targets/TARGET_Silicon_Labs/TARGET_EFM32/TARGET_EFM32GG/TARGET_EFM32GG_STK3700
		${MBED_DIR}/targets/TARGET_Silicon_Labs/TARGET_EFM32/TARGET_EFM32GG/device
		${MBED_DIR}/targets/TARGET_Silicon_Labs/TARGET_EFM32/TARGET_EFM32GG/device/TARGET_1024K
		${MBED_DIR}/targets/TARGET_Silicon_Labs/TARGET_EFM32/TARGET_EFM32GG/device/TARGET_1024K/TOOLCHAIN_GCC_ARM
		${MBED_DIR}/targets/TARGET_Silicon_Labs/TARGET_EFM32/common
		${MBED_DIR}/targets/TARGET_Silicon_Labs/TARGET_EFM32/emlib
		${MBED_DIR}/targets/TARGET_Silicon_Labs/TARGET_EFM32/emlib/inc
		${MBED_DIR}/targets/TARGET_Silicon_Labs/TARGET_EFM32/emlib/src
		${MBED_DIR}/targets/TARGET_Silicon_Labs/TARGET_EFM32/trng)

# Set mbed_cong.h file to be preprocessed by passing this to CMAKE_CXX_FLAGS, CMAKE_C_FLAGS
set(MBED_INCLUDES " -include mbed_config.h ")

set(CMAKE_EXE_LINKER_FLAGS " -Wl,--gc-sections -Wl,--wrap,main -Wl,--no-whole-archive  ")

# Set Include Directories
include_directories(${MBED_INCLUDE_DIR})
