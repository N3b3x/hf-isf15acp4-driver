set(HF_ISF15ACP4_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../.." CACHE PATH
    "Path to root of hf-isf15acp4-driver")

get_filename_component(HF_ISF15ACP4_ROOT "${HF_ISF15ACP4_ROOT}" ABSOLUTE)
if(NOT EXISTS "${HF_ISF15ACP4_ROOT}/cmake/hf_isf15acp4_build_settings.cmake")
    message(FATAL_ERROR "Cannot find ISF15ACP4 driver root at ${HF_ISF15ACP4_ROOT}")
endif()

include("${HF_ISF15ACP4_ROOT}/cmake/hf_isf15acp4_build_settings.cmake")

idf_component_register(
    SRCS ${HF_ISF15ACP4_SOURCE_FILES}
    INCLUDE_DIRS ${HF_ISF15ACP4_PUBLIC_INCLUDE_DIRS}
    REQUIRES ${HF_ISF15ACP4_IDF_REQUIRES}
)

target_compile_features(${COMPONENT_LIB} PRIVATE cxx_std_20)
