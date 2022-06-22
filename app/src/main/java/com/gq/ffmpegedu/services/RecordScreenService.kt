package com.gq.ffmpegedu.services

import android.app.Service
import android.content.Context
import android.content.Intent
import android.graphics.ImageFormat
import android.graphics.PixelFormat
import android.hardware.display.DisplayManager
import android.hardware.display.VirtualDisplay
import android.media.ImageReader
import android.media.MediaCodec
import android.media.projection.MediaProjection
import android.media.projection.MediaProjectionManager
import android.os.IBinder
import android.util.Log
import com.gq.ffmpegedu.native.X264Native
import com.gq.ffmpegedu.notification.NotificationBuilder
import java.text.Format

class RecordScreenService: Service() {

    private lateinit var mediaProjection: MediaProjection
    private val width = 1080
    private val height = 1920
    private lateinit var x264Native: X264Native

    companion object {
        const val ParamsResultCode = "ResultCode"
    }

    override fun onCreate() {
        super.onCreate()
        startFG()
    }

    override fun onBind(intent: Intent?): IBinder? {
        return null
    }

    override fun onStartCommand(intent: Intent, flags: Int, startId: Int): Int {
        x264Native = X264Native()
        x264Native.initX264Encoder(width, height)
        val resultCode = intent.getIntExtra(ParamsResultCode, -1)
        val projectionManager = getSystemService(Context.MEDIA_PROJECTION_SERVICE) as MediaProjectionManager

        val imageReader = ImageReader.newInstance(width, height, PixelFormat.RGBA_8888, 1)

        mediaProjection = projectionManager.getMediaProjection(resultCode, intent)
        mediaProjection.createVirtualDisplay("TAG-display",
            width, height,
            1,
            DisplayManager.VIRTUAL_DISPLAY_FLAG_AUTO_MIRROR,
            imageReader.surface,
            null,
            null)

        imageReader.setOnImageAvailableListener({ imageR: ImageReader? ->
            val acquireLatestImage = imageR?.acquireLatestImage()
            val buffer = acquireLatestImage?.planes?.get(0)?.buffer
            val byteArray = ByteArray(buffer?.remaining()!!)
            buffer.get(byteArray)
            x264Native.encoderRgba2Rtmp(byteArray)
            Log.d("------------------", "${buffer}")
            acquireLatestImage.close()
        }, null)

        return super.onStartCommand(intent, flags, startId)
    }


    /**
     * 启动前台通知
     */
    private fun startFG() {
        val builder = NotificationBuilder(this)
        val notification = builder.buildNotification(content = "RecordScreen")
        startForeground(NotificationBuilder.NOTIFICATION_HOSPITALRUSH_ID, notification)
    }
}