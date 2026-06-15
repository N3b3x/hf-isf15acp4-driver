#===============================================================================
# ISF15ACP4 Driver - Build Settings
# Shared variables for target name, includes, sources, and dependencies.
# This file is the SINGLE SOURCE OF TRUTH for the driver version.
#===============================================================================

include_guard(GLOBAL)

set(HF_ISF15ACP4_TARGET_NAME "hf_isf15acp4")

set(HF_ISF15ACP4_VERSION_MAJOR 1)
set(HF_ISF15ACP4_VERSION_MINOR 0)
set(HF_ISF15ACP4_VERSION_PATCH 0)
set(HF_ISF15ACP4_VERSION "${HF_ISF15ACP4_VERSION_MAJOR}.${HF_ISF15ACP4_VERSION_MINOR}.${HF_ISF15ACP4_VERSION_PATCH}")

set(HF_ISF15ACP4_VERSION_TEMPLATE "${CMAKE_CURRENT_LIST_DIR}/../inc/isf15acp4_version.h.in")
set(HF_ISF15ACP4_VERSION_HEADER_DIR "${CMAKE_CURRENT_BINARY_DIR}/hf_isf15acp4_generated")
set(HF_ISF15ACP4_VERSION_HEADER "${HF_ISF15ACP4_VERSION_HEADER_DIR}/isf15acp4_version.h")

file(MAKE_DIRECTORY "${HF_ISF15ACP4_VERSION_HEADER_DIR}")

if(EXISTS "${HF_ISF15ACP4_VERSION_TEMPLATE}")
    configure_file(
        "${HF_ISF15ACP4_VERSION_TEMPLATE}"
        "${HF_ISF15ACP4_VERSION_HEADER}"
        @ONLY
    )
    message(STATUS "ISF15ACP4 driver v${HF_ISF15ACP4_VERSION} — generated isf15acp4_version.h")
else()
    message(WARNING "isf15acp4_version.h.in not found at ${HF_ISF15ACP4_VERSION_TEMPLATE}")
endif()

set(HF_ISF15ACP4_PUBLIC_INCLUDE_DIRS
    "${CMAKE_CURRENT_LIST_DIR}/../inc"
    "${HF_ISF15ACP4_VERSION_HEADER_DIR}"
)

set(HF_ISF15ACP4_SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/../src/isf15acp4_font_data.cpp"
)

set(HF_ISF15ACP4_IDF_REQUIRES driver esp_driver_spi freertos)
