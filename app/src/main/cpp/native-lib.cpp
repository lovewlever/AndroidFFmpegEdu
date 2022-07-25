#include <jni.h>
#include <string>
#include "android/log.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <fdk-aac/aacenc_lib.h>
#include "x264/x264.h"
#include "lame/lame.h"
#include "openssl/crypto.h"
}

extern "C" JNIEXPORT jstring  JNICALL
Java_com_gq_ffmpegedu_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = avcodec_configuration();
    //std::string aacInfo = libInfo->versionStr;
    // fdk-aac
    HANDLE_AACENCODER aacEncoder;
    if (aacEncOpen(&aacEncoder, 0, MODE_1) != AACENC_OK) {
        printf("编码器实例分配失败");
    }
// x264
    x264_nal_t *nal = NULL;
    // x264
    x264_t *h = NULL;
    x264_param_t *param = (x264_param_t *) malloc(sizeof(x264_param_t));
    x264_picture_t *pic_in = (x264_picture_t *) (malloc(sizeof(x264_picture_t)));
    x264_picture_t *pic_out = (x264_picture_t *) (malloc(sizeof(x264_picture_t)));

    // mp3lame
    lame_global_flags *glf = lame_init();
    lame_print_config(glf);

    // openssl
    const char *version = OpenSSL_version(OPENSSL_VERSION);

    return env->NewStringUTF(hello.c_str());
}