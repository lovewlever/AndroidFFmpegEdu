//
// Created by catog on 2022/6/22.
//

#ifndef FFMPEGEDU_X264ENCODER_H
#define FFMPEGEDU_X264ENCODER_H
#define ANDROID_TAG  "X264Encoder"

#include "android/log.h"
#include <string>
extern "C" {
#include "includes/libavcodec/avcodec.h"
#include "includes/libavdevice/avdevice.h"
#include "includes/libavfilter/avfilter.h"
#include "includes/libavformat/avformat.h"
#include "includes/libavutil/avutil.h"
#include "includes/libavutil/imgutils.h"
#include "includes/libpostproc/postprocess.h"
#include "includes/libswresample/swresample.h"
#include "includes/libswscale/swscale.h"
};

class X264Encoder {
private:
    const AVCodec *codec = nullptr;
    AVStream *stream = nullptr;
    AVFormatContext *formatCtx = nullptr;
    SwsContext *swsContext = nullptr;
    AVFrame *frameRgba = nullptr;
    AVFrame *frameYuv = nullptr;
public:
    AVCodecContext *codecCtx = nullptr;
public:
    int pts = 0;
    int initEncoder(int width, int height);
    int initStream();
    int initFormat();
    int initRgbaToYuv420pSwsScale();
    void encoderRgbaToRtmp(uint8_t *rgbaByteArray);
};


#endif //FFMPEGEDU_X264ENCODER_H
