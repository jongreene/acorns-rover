# Generate a jlink script
function(make_jlink_script OUTPUT_DIR PROJECT_NAME MBED_APP_START_ADDRESS)
	# Set jlink's file path
	set(BOARD_JLINK_PATH "${OUTPUT_DIR}")
	# Jlink's flasher content
	set(JLINK_FILE_DATA "loadbin ${PLATFORM_ROOT}/${CMAKE_BUILD_TYPE}/bin/${PROJECT_NAME}.bin, ${MBED_APP_START_ADDRESS}\nr\ng\nqc\n")
	# Create new file and writes content based on JLINK_FILE_DATA
	file(WRITE "${BOARD_JLINK_PATH}/board.jlink" ${JLINK_FILE_DATA})
endfunction(make_jlink_script)

# Display memory map if SHOW_STATIC_MEMORY_MAP_ANALYSIS sets to true
# Note: This requires installing mbed-os python depencies
function(show_memory_map PROJECT_TARGET SHOW_MEMORY_MAP PROJECT_SOURCE_DIR PROJECT_NAME)
	if(${SHOW_MEMORY_MAP})
		# Show Static Memory Map Analysis using mbed-os memap.py
		add_custom_command(TARGET ${PROJECT_TARGET} POST_BUILD
				COMMAND python ../../lib/mbed-os/sources/mbed-os-5.9.7/tools/memap.py ${CMAKE_CURRENT_BINARY_DIR}/../${PROJECT_NAME}.map -t GCC_ARM
				)
	endif()
endfunction(show_memory_map)

# Custom command to use objcopy to create .bin files out of ELF files
function(make_mbed_firmware INPUT)
	add_custom_command(TARGET ${INPUT}
			COMMAND ${GCC_DIR}/bin/arm-none-eabi-objcopy -O binary ${INPUT} ${INPUT}.bin
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "objcopying to make mbed compatible firmware")
	set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${PLATFORM_ROOT}/${CMAKE_BUILD_TYPE}/${INPUT}.bin)
endfunction(make_mbed_firmware)