#ifndef TELLO_VIDEO_H
#define TELLO_VIDEO_H

// This is needed because MSVC is weird and requires you to explicitly link windows libraries
#pragma comment(lib, "wsock32")
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "crypt32")
#pragma comment(lib, "advapi32")
#pragma comment(lib, "ole32")
#pragma comment(lib, "cfgmgr32")
#pragma comment(lib, "secur32")
#pragma comment(lib, "bcrypt")


#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include <chrono>


extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#define ENABLE_LOGGING false

namespace Tello {

class Video
{
private:
    AVFormatContext* stream_context;
    AVPacket packet;
    AVFormatContext* allocation_context;
    AVStream* stream;
    AVCodec *codec;
    AVCodecContext* video_context;

    SwsContext *yuv420p_to_rgb24_ctx; // This converts YUV420P to RGB24

    std::vector<uint8_t> yuv420p_picturebuffer;
    std::vector<uint8_t> rgb24_picturebuffer;

    AVFrame* yuv420p_frame;
    AVFrame* rgb24_frame;

    int frame_count = 0;
    int video_stream_index = -1; // This is the index of the video within the h264 UDP stream

    std::thread t1;
    std::atomic_bool stop_thread;

public:
    Video(std::string url);
    ~Video();

    void Decode_Stream();
};

}

#endif // TELLO_VIDEO_H
