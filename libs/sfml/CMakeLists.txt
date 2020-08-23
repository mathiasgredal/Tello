FetchContent_GetProperties(SFML)

if(NOT SFML_POPULATED)
        set(SFML_STATIC_LIBRARIES TRUE)


	set(SFML_INSTALL_PKGCONFIG_FILES OFF)

	FetchContent_Populate(SFML)

	# No need to build audio and network modules
	set(SFML_BUILD_AUDIO FALSE)
	set(SFML_BUILD_NETWORK FALSE)
	add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR} EXCLUDE_FROM_ALL)

endif()
