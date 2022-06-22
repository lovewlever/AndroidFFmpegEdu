package com.gq.ffmpegedu.notification

import android.app.Notification
import android.app.NotificationManager
import androidx.core.app.NotificationCompat
import androidx.annotation.RequiresApi
import android.os.Build
import android.app.NotificationChannel
import android.content.Context
import com.gq.ffmpegedu.R

internal class NotificationBuilder(private val mContext: Context) {
    private val mNotificationManager: NotificationManager = mContext.getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager

    fun createChannel() {
        if (shouldCreateNowPlayingChannel()) {
            createNowPlayingChannel()
        }
    }

    fun buildNotification(title: String = "", content: String): Notification {
        createChannel()
        val builder = NotificationCompat.Builder(mContext, NOTIFICATION_CHANNEL_ID)
        return builder.setSmallIcon(R.drawable.ic_launcher_background)
            .setContentTitle(title)
            .setContentText(content)
            .setOnlyAlertOnce(true)
            .setVisibility(NotificationCompat.VISIBILITY_PUBLIC)
            .build()
    }

    @RequiresApi(api = Build.VERSION_CODES.O)
    private fun nowPlayingChannelExists(): Boolean {
        return mNotificationManager.getNotificationChannel(NOTIFICATION_CHANNEL_ID) != null
    }

    private fun shouldCreateNowPlayingChannel(): Boolean {
        return Build.VERSION.SDK_INT >= Build.VERSION_CODES.O && !nowPlayingChannelExists()
    }

    @RequiresApi(api = Build.VERSION_CODES.O)
    private fun createNowPlayingChannel() {
        val channel = NotificationChannel(
            NOTIFICATION_CHANNEL_ID,
            "AutoOpenUsb",
            NotificationManager.IMPORTANCE_LOW
        )
        channel.description = "AutoOpenUsb"
        mNotificationManager.createNotificationChannel(channel)
    }

    companion object {
        const val NOTIFICATION_CHANNEL_ID = "com.gq.transmission"
        const val NOTIFICATION_HOSPITALRUSH_ID = 0xD660
        const val NOTIFICATION_FLOATING_ID = 0xD660
    }

}