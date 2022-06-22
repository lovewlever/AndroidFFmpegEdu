//
// Created by catog on 2022/6/22.
//


#include "X264Encoder.h"
#include <jni.h>


int X264Encoder::initEncoder(int width, int height) {
    codec = avcodec_find_encoder(AV_CODEC_ID_MPEG4);
    codecCtx = avcodec_alloc_context3(codec);
    codecCtx->width = width;
    codecCtx->height = height;
    codecCtx->framerate = { 30, 1};
    codecCtx->time_base = { 1, 30 };
    codecCtx->gop_size = 100;
    codecCtx->bit_rate = width * height;
    codecCtx->max_b_frames = 0;
    codecCtx->codec_id = codec->id;
    codecCtx->codec_type = codec->type;
    codecCtx->level = 50;
    codecCtx->pix_fmt = AV_PIX_FMT_YUV420P;

    int ret = avcodec_open2(codecCtx, codec, nullptr);

    if (ret < 0) {
        __android_log_print(ANDROID_LOG_DEBUG, ANDROID_TAG, "%s", std::to_string(ret).c_str());
        return ret;
    }

    return 0;

}

int X264Encoder::initFormat() {
    int ret = avformat_alloc_output_context2(&formatCtx, nullptr, "flv", "192.168.3.217:1935");
    if (ret < 0) {
        __android_log_print(ANDROID_LOG_DEBUG, ANDROID_TAG, "%s", std::to_string(ret).c_str());
        return ret;
    }
    return 0;
}

int X264Encoder::initStream() {
    stream = avformat_new_stream(formatCtx, codec);
    int ret = avcodec_parameters_from_context(stream->codecpar, codecCtx);
    if (ret < 0) {
        __android_log_print(ANDROID_LOG_DEBUG, ANDROID_TAG, "%s", std::to_string(ret).c_str());
        return ret;
    }
    avformat_write_header(formatCtx, nullptr);
    return 0;
}

int X264Encoder::initRgbaToYuv420pSwsScale() {

    swsContext = sws_getContext(codecCtx->width, codecCtx->height, AV_PIX_FMT_RGBA, codecCtx->width,
                                            codecCtx->height, codecCtx->pix_fmt, SWS_FAST_BILINEAR,
                                            nullptr, nullptr, nullptr);

    frameRgba = av_frame_alloc();
    frameYuv = av_frame_alloc();
    int rgbaSize = av_image_get_buffer_size(AV_PIX_FMT_RGBA, codecCtx->width, codecCtx->height, 1);
    uint8_t * rgbaBuf = (uint8_t *)av_malloc(rgbaSize);
    int ret = av_image_fill_arrays(frameRgba->data, frameRgba->linesize, rgbaBuf ,AV_PIX_FMT_RGBA, codecCtx->width, codecCtx->height, 1);

    if (ret <= 0) {
        return ret;
    }

    int yuvSize = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, codecCtx->width, codecCtx->height, 1);
    uint8_t * yuvBuf = (uint8_t *)av_malloc(yuvSize);
    ret = av_image_fill_arrays(frameYuv->data, frameYuv->linesize, yuvBuf ,AV_PIX_FMT_YUV420P, codecCtx->width, codecCtx->height, 1);
    if (ret <= 0) {
        return ret;
    }

    return 0;
}

void X264Encoder::encoderRgbaToRtmp(uint8_t *rgbaByteArray) {

    frameRgba->data[0] = rgbaByteArray;
    int ret = sws_scale(swsContext, frameRgba->data, frameRgba->linesize, 0, codecCtx->height, frameYuv->data, frameYuv->linesize);
    if (ret <= 0) {

    }

    AVPacket *packet = av_packet_alloc();
    ret = avcodec_send_frame(codecCtx, frameYuv);
    while (ret >= 0) {
        ret = avcodec_receive_packet(codecCtx, packet);
        packet->pts = pts;
        av_packet_rescale_ts(packet, codecCtx->time_base , stream->time_base);
        av_interleaved_write_frame(formatCtx, packet);
        av_packet_unref(packet);
        pts ++;
    }

}