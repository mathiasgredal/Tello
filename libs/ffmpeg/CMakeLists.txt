# TODO: make this crossplatform
# Precompiled windows ffmpeg libs from here: https://github.com/FutaAlice/ffmpeg-static-libs/releases/tag/3.4.2

#set(FFMPEG_ARCHIVE ${CMAKE_CURRENT_SOURCE_DIR}/download.7z)

#if(NOT EXISTS ${FFMPEG_ARCHIVE})
#    message(STATUS "Downloading FFMPEG")
#    file(DOWNLOAD https://github.com/FutaAlice/ffmpeg-static-libs/releases/download/3.4.2/ffmpeg_3.4.2.7z
#        ${FFMPEG_ARCHIVE}
#         TIMEOUT 60  # seconds
#         TLS_VERIFY ON)

#file(ARCHIVE_EXTRACT INPUT ${FFMPEG_ARCHIVE} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

# file(GLOB FFMPEG_FILES
#   "ffmpeg_3.4.2/*"
# )
# file(COPY ${FFMPEG_FILES} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

#endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Debug libraries
    set(FFMPEG_LIBRARIES
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/avcodec.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/avdevice.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/avfilter.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/avformat.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/avresample.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/avutil.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/bz2d.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/fdk-aac.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/libcharset.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/libcrypto.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/libiconv.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/libmp3lame-static.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/libmpghip-static.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/libspeexd.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/libssl.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/libwavpack.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/libx264.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/lzmad.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/ogg.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/OpenCL.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/opus.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/postproc.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/SDL2d.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/snappyd.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/soxr.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/swresample.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/swscale.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/theora.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/theoradec.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/theoraenc.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/vorbis.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/vorbisenc.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/vorbisfile.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/vpxmtd.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/zlibd.lib
        wsock32
        ws2_32
        crypt32
        advapi32
        ole32
        cfgmgr32
        PARENT_SCOPE
        )
else(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # release libraries
    set(FFMPEG_LIBRARIES
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/avcodec.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/avdevice.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/avfilter.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/avformat.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/avresample.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/avutil.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/bz2.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/fdk-aac.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/libcharset.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/libcrypto.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/libiconv.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/libmp3lame-static.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/libmpghip-static.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/libspeex.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/libssl.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/libwavpack.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/libx264.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/lzma.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/ogg.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/OpenCL.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/opus.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/postproc.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/SDL2.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/snappy.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/soxr.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/swresample.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/swscale.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/theora.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/theoradec.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/theoraenc.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/vorbis.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/vorbisenc.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/vorbisfile.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/vpxmt.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/zlib.lib
        wsock32
        ws2_32
        crypt32
        advapi32
        ole32
        cfgmgr32
        PARENT_SCOPE
        )
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

set(FFMPEG_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

