# Tello
To compile this project, you must unzip the static libraries in ``libs/ffmpeg/lib.7z`` or compile the libraries yourself. After that you can just compile it like any other cmake based project.



### Compiling ffmpeg yourself
If you compile ffmpeg yourself, then you should be able to use any recent version of ffmpeg. The libraries in the zip folder are made with the following settings:
```
ffmpeg version 4.2 Copyright (c) 2000-2019 the FFmpeg developers
  built with Microsoft (R) C/C++ Optimizing Compiler Version 19.27.29111 for x64
  configuration: --toolchain=msvc --prefix=/f/src/vcpkg/packages/ffmpeg_x64-windows-static --enable-asm --enable-yasm 
                 --disable-doc --enable-debug --enable-runtime-cpudetect --enable-nonfree --enable-gpl --enable-version3 
                 --enable-ffmpeg --enable-ffplay --enable-ffprobe --enable-avcodec --enable-avdevice --enable-avformat 
                 --enable-avfilter --enable-postproc --enable-swresample --enable-swscale --enable-avresample --disable-avisynth 
                 --enable-bzlib --enable-iconv --enable-libfdk-aac --enable-lzma --enable-libmp3lame --enable-cuda 
                 --enable-nvenc --enable-nvdec --enable-cuvid --enable-opencl --enable-openssl --enable-libopus 
                 --enable-sdl2 --enable-libsnappy --enable-libsoxr --enable-libspeex --enable-libtheora --enable-libvorbis 
                 --enable-libvpx --disable-libwavpack --enable-libx264 --disable-libx265 --enable-zlib 
                 --extra-cflags='-DHAVE_UNISTD_H=0' --pkg-config-flags=--static --extra-cflags=-MT --extra-cxxflags=-MT
  libavutil      56. 31.100 / 56. 31.100
  libavcodec     58. 54.100 / 58. 54.100
  libavformat    58. 29.100 / 58. 29.100
  libavdevice    58.  8.100 / 58.  8.100
  libavfilter     7. 57.100 /  7. 57.100
  libavresample   4.  0.  0 /  4.  0.  0
  libswscale      5.  5.100 /  5.  5.100
  libswresample   3.  5.100 /  3.  5.100
  libpostproc    55.  5.100 / 55.  5.100
```

