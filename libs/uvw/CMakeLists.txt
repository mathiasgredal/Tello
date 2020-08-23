FetchContent_GetProperties(UVW)

if(NOT UVW_POPULATED)

	set(BUILD_UVW_LIBS ON CACHE INTERNAL "") # This configures uvw to build static libraries

	message(STATUS "Fetching UVW ${UVW_VERSION}")

	FetchContent_Populate(UVW)

	add_subdirectory(${uvw_SOURCE_DIR} ${uvw_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
