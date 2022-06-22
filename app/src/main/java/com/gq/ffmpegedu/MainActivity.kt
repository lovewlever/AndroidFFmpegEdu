package com.gq.ffmpegedu

import android.app.Activity
import android.content.Context
import android.media.projection.MediaProjectionManager
import android.os.Build
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.provider.SyncStateContract
import android.widget.TextView
import androidx.activity.result.ActivityResult
import androidx.activity.result.contract.ActivityResultContracts
import androidx.annotation.RequiresApi
import com.gq.ffmpegedu.databinding.ActivityMainBinding
import com.gq.ffmpegedu.native.X264Native
import com.gq.ffmpegedu.services.RecordScreenService

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    @RequiresApi(Build.VERSION_CODES.O)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.sampleText.text = stringFromJNI()
        val projectionManager =
            getSystemService(Context.MEDIA_PROJECTION_SERVICE) as MediaProjectionManager
        val createScreenCaptureIntent = projectionManager.createScreenCaptureIntent()
        registerForActivityResult(
            ActivityResultContracts.StartActivityForResult()
        ) { result: ActivityResult? ->
            result?.data?.let { intent ->
                startForegroundService(intent.also { ent ->
                    ent.setClass(this, RecordScreenService::class.java)
                    ent.putExtra(RecordScreenService.ParamsResultCode, result.resultCode)
                })
            }
        }.launch(createScreenCaptureIntent)
    }

    /**
     * A native method that is implemented by the 'ffmpegedu' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'ffmpegedu' library on application startup.
        init {
            System.loadLibrary("ffmpegedu")
        }
    }
}