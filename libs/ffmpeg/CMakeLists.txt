# TODO: make this crossplatform
# Precompiled windows ffmpeg libs from here: https://github.com/FutaAlice/ffmpeg-static-libs/releases/tag/3.4.2

set(FFMPEG_ARCHIVE ${CMAKE_CURRENT_SOURCE_DIR}/download.7z)

if(NOT EXISTS ${FFMPEG_ARCHIVE})
    message(STATUS "Downloading FFMPEG")
    file(DOWNLOAD https://github.com/FutaAlice/ffmpeg-static-libs/releases/download/3.4.2/ffmpeg_3.4.2.7z
        ${FFMPEG_ARCHIVE}
         TIMEOUT 60  # seconds
         TLS_VERIFY ON)

 file(ARCHIVE_EXTRACT INPUT ${FFMPEG_ARCHIVE} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

 file(GLOB FFMPEG_FILES
   "ffmpeg_3.4.2/*"
 )
 file(COPY ${FFMPEG_FILES} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Debug libraries
    set(FFMPEG_LIBRARIES
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/libavcodecd.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/libavdeviced.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/libavfilterd.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/libavformatd.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/libavutild.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/libpostprocd.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/libswresampled.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/libswscaled.lib
        PARENT_SCOPE
        )
else(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # release libraries
    set(FFMPEG_LIBRARIES
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/libavcodec.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/libavdevice.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/libavfilter.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/libavformat.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/libavutil.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/libpostproc.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/libswresample.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/libswscale.lib
        PARENT_SCOPE
        )
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

set(FFMPEG_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

