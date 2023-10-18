#include "play_url.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

#include "../defined.h"

void play_audio_url(char *url)
{
    char command[256];
    sprintf(command, "ffplay -vn -sn -nodisp -autoexit %s", url);
    system(command);
}

#if 0
void play_audio_url(char* url) {
       // Initialize FFmpeg
    // av_register_all();

    // Open the input file
    AVFormatContext* format_ctx = NULL;
    if (avformat_open_input(&format_ctx, url, NULL, NULL) != 0) {
        ERROR << "Error opening input file: " << url;
        return;
    }

    // Find the audio stream
    int audio_stream_index = -1;
    for (unsigned int i = 0; i < format_ctx->nb_streams; i++) {
        if (format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
            break;
        }
    }
    if (audio_stream_index == -1) {
        ERROR << "Error finding audio stream";
        return;
    }

    // Get the audio codec parameters
    AVCodecParameters* codec_params = format_ctx->streams[audio_stream_index]->codecpar;

    // Find the audio codec
    const AVCodec* codec = avcodec_find_decoder(codec_params->codec_id);
    if (codec == NULL) {
        ERROR << "Error finding audio codec";
        return;
    }

    // Open the audio codec
    AVCodecContext* codec_ctx = avcodec_alloc_context3(codec);
    if (avcodec_parameters_to_context(codec_ctx, codec_params) != 0) {
        ERROR << "Error setting codec parameters";
        return;
    }
    if (avcodec_open2(codec_ctx, codec, NULL) != 0) {
        ERROR << "Error opening audio codec";
        return;
    }

    // Allocate the audio frame
    AVFrame* frame = av_frame_alloc();
    if (frame == NULL) {
        ERROR << "Error allocating audio frame";
        return;
    }

    // Allocate the resampled audio frame
    AVFrame* resampled_frame = av_frame_alloc();
    if (resampled_frame == NULL) {
        ERROR << "Error allocating resampled audio frame";
        return;
    }

    // Allocate the resampler context
    SwrContext* resampler_ctx = swr_alloc();
    if (resampler_ctx == NULL) {
        ERROR << "Error allocating resampler context";
        return;
    }

    // Set the resampler options
    av_opt_set_int(resampler_ctx, "in_channel_layout", codec_ctx->channel_layout, 0);
    av_opt_set_int(resampler_ctx, "in_sample_rate", codec_ctx->sample_rate, 0);
    av_opt_set_sample_fmt(resampler_ctx, "in_sample_fmt", codec_ctx->sample_fmt, 0);
    av_opt_set_int(resampler_ctx, "out_channel_layout", AV_CH_LAYOUT_STEREO, 0);
    av_opt_set_int(resampler_ctx, "out_sample_rate", 44100, 0);
    av_opt_set_sample_fmt(resampler_ctx, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);

    // Initialize the resampler context
    if (swr_init(resampler_ctx) != 0) {
        ERROR << "Error initializing resampler context";
        return;
    }

    // Initialize the packet
    AVPacket packet;
    av_init_packet(&packet);

    // Read packets from the input file and decode them
    while (av_read_frame(format_ctx, &packet) == 0) {
        if (packet.stream_index == audio_stream_index) {
            // Decode the audio packet
            int ret = avcodec_send_packet(codec_ctx, &packet);
            if (ret < 0) {
                ERROR << "Error sending audio packet";
                return;
            }
            while (ret >= 0) {
                ret = avcodec_receive_frame(codec_ctx, frame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    break;
                } else if (ret < 0) {
                    ERROR << "Error receiving audio frame";
                    return;
                }

                // Resample the audio frame
                swr_convert(resampler_ctx, resampled_frame->data, resampled_frame->nb_samples, (const uint8_t**)frame->data, frame->nb_samples);

                // Play the audio frame
                // TODO: Implement audio playback
            }
        }
        av_packet_unref(&packet);
    }

    // Free resources
    av_frame_free(&frame);
    av_frame_free(&resampled_frame);
    swr_free(&resampler_ctx);
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&format_ctx); 
}
#endif