#include <jni.h>
#include <string>
#include "android/log.h"

extern "C" {
#include <libavcodec/avcodec.h>
}

extern "C" JNIEXPORT jstring  JNICALL
Java_com_gq_ffmpegedu_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = avcodec_configuration();
    return env->NewStringUTF(hello.c_str());
}