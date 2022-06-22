package com.gq.ffmpegedu.native

class X264Native {

    external fun initX264Encoder(width: Int, height: Int): Int

    external fun encoderRgba2Rtmp(yuvByteArray: ByteArray)

}