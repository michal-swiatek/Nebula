set(NEBULA_MAJOR_VERSION 1)
set(NEBULA_MINOR_VERSION 0)
set(NEBULA_PATCH_VERSION 0)
set(NEBULA_INITIALIZATION_VERBOSITY 1)  # 0 - No messages, 1 - init messages, 2 - extended init messages

configure_file(include/platform/EngineConfiguration.h.in include/platform/EngineConfiguration.h @ONLY)