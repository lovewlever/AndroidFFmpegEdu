//
// Created by catog on 2022/6/22.
//

#include <jni.h>
#include <string>
#include "android/log.h"

#include "X264Encoder.h"

X264Encoder *x264Encoder = nullptr;

extern "C"
JNIEXPORT jint JNICALL
Java_com_gq_ffmpegedu_native_X264Native_initX264Encoder(JNIEnv *env, jobject thiz, jint width, jint height) {
    x264Encoder = new X264Encoder();
    x264Encoder->initEncoder(width, height);
    x264Encoder->initFormat();
    x264Encoder->initStream();
    x264Encoder->initRgbaToYuv420pSwsScale();
    return 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gq_ffmpegedu_native_X264Native_encoderRgba2Rtmp(JNIEnv *env, jobject thiz,
                                                     jbyteArray yuv_byte_array) {
    jbyte *jb = env->GetByteArrayElements(yuv_byte_array, JNI_FALSE);
    uint8_t *rgbaByte = (uint8_t *)jb;
    x264Encoder->encoderRgbaToRtmp(rgbaByte);
}