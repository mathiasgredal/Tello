#include "Tello_Video.h"

void Logging_Callback(void *ptr, int level, const char *fmt, va_list vargs)
{
    static char message[8192];
    const char *module = NULL;

    if (ptr)
    {
        AVClass *avc = *(AVClass**) ptr;
        module = avc->item_name(ptr);
    }
    vsnprintf_s(message, sizeof(message), fmt, vargs);

    std::cout << "LOG: " << message << std::endl;
}

Tello::Video::Video(std::string url)
{
    stream_context = avformat_alloc_context();

    // This is supposed to speed up connections

#if ENABLE_LOGGING
    av_log_set_callback(&Logging_Callback);
#endif
    std::cout << "A" << std::endl;
    if(avformat_open_input(&stream_context, url.c_str(), NULL, NULL))
        throw std::runtime_error("ERROR: Unable to connect to url: " + url);
    std::cout << "B" << std::endl;

    if(avformat_find_stream_info(stream_context, NULL) < 0)
        throw std::runtime_error("ERROR: Unable to find stream info");

    std::cout << "C" << std::endl;

    for(uint32_t i =0; i < stream_context->nb_streams; i++){
        if(stream_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
            video_stream_index = i;
    }
    std::cout << "D" << std::endl;

    if(video_stream_index == -1)
        throw std::runtime_error("ERROR: Unable to find video in requested stream");
    std::cout << "E" << std::endl;

    av_init_packet(&packet);
    std::cout << "F" << std::endl;

    allocation_context = avformat_alloc_context();
    std::cout << "G" << std::endl;

    // Now we can start reading the video packets
    av_read_play(stream_context);
    std::cout << "H" << std::endl;

    // We know the Tello drone sends an encoded h264 stream
    codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!codec)
        throw std::runtime_error("ERROR: The linked ffmpeg library seems not to have an h264 decoder");
    std::cout << "I" << std::endl;

    video_context = avcodec_alloc_context3(codec);
    std::cout << "J" << std::endl;

    avcodec_get_context_defaults3(video_context, codec);
    avcodec_copy_context(video_context,stream_context->streams[video_stream_index]->codec);
    std::cout << "A" << std::endl;

    if (avcodec_open2(video_context, codec, nullptr) < 0)
        throw std::runtime_error("ERROR: Unable to initialize the video context with the given AVCodec");
    std::cout << "A" << std::endl;

    // Initializing the converter that will convert the decoded YUV420P frames to RGB24 frames that we can render with SFML
    yuv420p_to_rgb24_ctx = sws_getContext(video_context->width, video_context->height, video_context->pix_fmt, video_context->width, video_context->height,
                                        AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);

    int yuv420p_size = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, video_context->width, video_context->height, 1);
    yuv420p_picturebuffer.resize(yuv420p_size);

    int rgb24_size = av_image_get_buffer_size(AV_PIX_FMT_RGB24, video_context->width, video_context->height, 1);
    rgb24_picturebuffer.resize(rgb24_size);

    yuv420p_frame = av_frame_alloc();
    av_image_fill_arrays(yuv420p_frame->data, yuv420p_frame->linesize, yuv420p_picturebuffer.data(), AV_PIX_FMT_YUV420P, video_context->width, video_context->height, 1);

    rgb24_frame = av_frame_alloc();
    av_image_fill_arrays(rgb24_frame->data, rgb24_frame->linesize, rgb24_picturebuffer.data(), AV_PIX_FMT_RGB24, video_context->width, video_context->height, 1);

    // Now most of the preliminary work is done, so now we can fire up a new thread for decoding
    t1 = std::thread([this]() {Decode_Stream();});
}

Tello::Video::~Video()
{
    // Close thread
    stop_thread = true;
    t1.join();

    // Clear heap
    av_free(yuv420p_frame);
    av_free(rgb24_frame);
    av_read_pause(stream_context);
    avio_close(allocation_context->pb);
    avformat_free_context(allocation_context);

    std::cout << "Video destroyed" << std::endl;
}

void Tello::Video::Decode_Stream()
{
    uint64_t time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    while(av_read_frame(stream_context, &packet)>=0 && stop_thread != true)
    {
        // Check whether this packet belongs to the videostream
        if(packet.stream_index == video_stream_index){

            if(stream == NULL) { // This will only be run once
                stream = avformat_new_stream(allocation_context, stream_context->streams[video_stream_index]->codec->codec);
                avcodec_copy_context(stream->codec, stream_context->streams[video_stream_index]->codec);
                stream->sample_aspect_ratio = stream_context->streams[video_stream_index]->codec->sample_aspect_ratio;
            }

            int check = 0;
            packet.stream_index = stream->id;

            avcodec_decode_video2(video_context, yuv420p_frame, &check, &packet);

            if(check)
            {
                // We have succesfully decoded a frame
                sws_scale(yuv420p_to_rgb24_ctx, yuv420p_frame->data, yuv420p_frame->linesize, 0, video_context->height, rgb24_frame->data, rgb24_frame->linesize);

                // Update sfml video image thing
                std::cout << "Frame: " << frame_count << ", FPS: " << frame_count/(float)(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()-time) << std::endl;
                frame_count++;
            }

        }

        av_packet_unref(&packet);
        av_init_packet(&packet);
    }

    std::cout << "Decoder stopped" << std::endl;

}
