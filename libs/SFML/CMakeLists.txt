FetchContent_GetProperties(SFML)

if(NOT SFML_POPULATED)
  option(SFML_STATIC_LIBRARIES "" TRUE)
  option(SFML_INSTALL_PKGCONFIG_FILES "" FALSE)

  if (USE_SYSTEM_DEPS)
    find_package(SFML ${SFML_VERSION} COMPONENTS graphics QUIET)
  else()
    FetchContent_Populate(SFML)

    # No need to build audio and network modules
    option(SFML_BUILD_AUDIO "" FALSE)
    option(SFML_BUILD_NETWORK "" FALSE)
    add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR} EXCLUDE_FROM_ALL)
  endif()
endif()
