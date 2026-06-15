# ESP32 app selection is driven by scripts/build_app.sh via APP_SOURCE_FILE.

if(NOT DEFINED APP_SOURCE_FILE)
    set(APP_SOURCE_FILE "${DEFAULT_SOURCE_FILE}" CACHE STRING "Selected app source file")
endif()

idf_component_register(
    SRCS "${APP_SOURCE_FILE}"
    INCLUDE_DIRS "."
    REQUIRES hf_isf15acp4 driver esp_timer
)

target_compile_features(${COMPONENT_LIB} PRIVATE cxx_std_20)
