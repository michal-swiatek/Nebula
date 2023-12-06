set(NEBULA_MAJOR_VERSION 1)
set(NEBULA_MINOR_VERSION 0)
set(NEBULA_PATCH_VERSION 0)

set(NEBULA_INITIALIZATION_VERBOSITY 2)  # 0 - No messages, 1 - low, 2 - normal, 3 - high
set(NEBULA_RESOURCES_DIRECTORY "resources")

configure_file(include/platform/EngineConfiguration.h.in include/platform/EngineConfiguration.h @ONLY)