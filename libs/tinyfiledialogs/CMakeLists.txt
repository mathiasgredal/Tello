FetchContent_GetProperties(TINY_FILE_DIALOGS)

if(NOT TINY_FILE_DIALOGS_POPULATED)
	message(STATUS "Fetching TINY_FILE_DIALOGS ${TINY_FILE_DIALOGS_VERSION}")

	FetchContent_Populate(TINY_FILE_DIALOGS)

        add_library(tinyfiledialogs STATIC ${tiny_file_dialogs_SOURCE_DIR}/tinyfiledialogs.c ${tiny_file_dialogs_SOURCE_DIR}/tinyfiledialogs.h)

        if(WIN32)
            target_link_libraries(tinyfiledialogs comdlg32 ole32)
        endif()

	target_include_directories(tinyfiledialogs PUBLIC ${tiny_file_dialogs_SOURCE_DIR})
endif()
