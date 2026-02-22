
cmake_minimum_required(VERSION 3.16)
project(TXSEX_MOCKBA3_6)

# --- SETTINGS ---
set(BIN_NAME "txsex")
set(ZIP_NAME "txSex_Addon_v0.1.zip")
set(FORCE_HOST "akai-force")
set(FORCE_DEST "/media/662522/AddOns/txSex/")
set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set distribution directory
set(DIST_DIR ${CMAKE_SOURCE_DIR}/AddOns/txSex)

# Set output directory to build folder (not final destination)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compiler flags from your shell script
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fPIC -Wno-unused-variable -w")

# Define for ALSA support (from your shell script: -D__LINUX_ALSA__)
add_definitions(-D__LINUX_ALSA__)

# Cross-compilation settings for ARM
if(CMAKE_CROSSCOMPILING)
    # ARM-specific flags for Akai Force
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv7-a -mtune=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard")
endif()

# Source files
set(SOURCE_FILES
        src/main.cpp
        src/RtMidi.cpp
        src/RtMidi.h
        src/RtError.h
)

# Create executable
add_executable(${BIN_NAME} ${SOURCE_FILES})

# Link libraries (from your shell script)
target_link_libraries(${BIN_NAME}
        m           # -lm
        dl          # -ldl
        stdc++      # -lstdc++
        asound      # -lasound
        pthread     # -lpthread
)

# Post-build custom command: strip, package, and deploy
add_custom_command(TARGET ${BIN_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Post-processing ${BIN_NAME}..."
        COMMAND /usr/local/bin/deploy_force.sh $<TARGET_FILE:${BIN_NAME}> ${DIST_DIR} ${ZIP_NAME} ${FORCE_HOST} ${FORCE_DEST}
        COMMENT "Processing ${BIN_NAME} build, package, and deploy to ${FORCE_HOST}"
        VERBATIM
)

# Installation rules (optional)
install(TARGETS ${BIN_NAME} DESTINATION bin)