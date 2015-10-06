package com.cgroup.androidvision;

import android.content.Context;
import android.content.Intent;
import android.hardware.display.DisplayManager;
import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaFormat;
import android.media.MediaMuxer;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnPreparedListener;
import android.media.projection.MediaProjection;
import android.media.projection.MediaProjectionManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.support.v7.app.AppCompatActivity;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.VideoView;

import java.io.IOException;
import java.net.SocketException;
import java.nio.ByteBuffer;
import java.text.SimpleDateFormat;
import java.util.Date;

/** VideoActivity is the user interface in which user can watch the video captured by the camera
 * and user can also turn the camera to the left side or the right side.
 */
public class VideoActivity extends AppCompatActivity {

    private static final int REQUEST_CODE_CAPTURE_PERM = 1234;

    /** Variable Declaration */
    private VideoView mVideoView;
    protected String ipAddress = "10.0.0.1";
    protected int port = 2016;
    protected UDPClientClass udpClientClass;
    protected byte[] data = new byte[3];
    protected int position = 0;
    protected byte id = (byte) 0x03;
    protected byte d1 = (byte) 0;
    protected byte d2 = (byte) 0;
    protected boolean isRightThreadOn = false;
    protected boolean isLeftThreadOn = false;
    protected String url = "rtsp://10.0.0.1:8554/test";
    protected boolean isPlaying = false;
    protected boolean isRecording = false;
    protected TextView positionText;

    protected Button playButton;
    protected Button recordButton;

    private MediaProjectionManager mMediaProjectionManager;

    private static final String VIDEO_MIME_TYPE = "video/avc";
    private static final int VIDEO_WIDTH = 1920;
    private static final int VIDEO_HEIGHT = 1080;

    private boolean mMuxerStarted = false;
    private MediaProjection mMediaProjection;
    private Surface mInputSurface;
    private MediaMuxer mMuxer;
    private MediaCodec mVideoEncoder;
    private MediaCodec.BufferInfo mVideoBufferInfo;
    private int mTrackIndex = -1;

    private final Handler mDrainHandler = new Handler(Looper.getMainLooper());
    private Runnable mDrainEncoderRunnable = new Runnable() {
        @Override
        public void run() {
            drainEncoder();
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        try {
            udpClientClass = new UDPClientClass();
        } catch (SocketException e) {
            e.printStackTrace();
        }
        setContentView(R.layout.activity_video);

        positionText = (TextView) findViewById(R.id.textControl);

        mVideoView = (VideoView) findViewById(R.id.video);

        playButton = (Button) findViewById(R.id.buttonPlay);

        recordButton = (Button) findViewById(R.id.recordButton);
        recordButton.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_UP) {
                    if (!isRecording) {
                        Intent permissionIntent = mMediaProjectionManager.createScreenCaptureIntent();
                        startActivityForResult(permissionIntent, REQUEST_CODE_CAPTURE_PERM);
                        isRecording = true;
                        recordButton.setBackgroundResource(R.drawable.stop_rec_transparent);
                    } else {
                        releaseEncoders();
                        isRecording = false;
                        recordButton.setBackgroundResource(R.drawable.rec_transparent);
                    }

                }
                return false;
            }
        });


        mVideoView.setOnPreparedListener(new OnPreparedListener() {
            @Override
            public void onPrepared(MediaPlayer mp) {
                Log.d("com.ricardo.teste", "prepared");
                mVideoView.start();
                isPlaying = true;
                VideoActivity.this.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        playButton.setBackgroundResource(R.drawable.stop_transparent);
                    }
                });
            }
        });

        Button right = (Button) findViewById(R.id.buttonRight);
        right.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    isRightThreadOn = true;
                    rightThread();
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    isRightThreadOn = false;
                }
                return false;
            }
        });

        Button left = (Button) findViewById(R.id.buttonLeft);
        left.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    isLeftThreadOn = true;
                    leftThread();
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    isLeftThreadOn = false;
                }
                return false;
            }
        });

        mMediaProjectionManager = (MediaProjectionManager) getSystemService(Context.MEDIA_PROJECTION_SERVICE);
    }

    /** It will ask the user for the authorization for recording the video (screen capture)
     * @param requestCode int
     * @param resultCode int
     * @param intent Intent
     */
    public void onActivityResult(int requestCode, int resultCode, Intent intent) {
        if (REQUEST_CODE_CAPTURE_PERM == requestCode) {
            if (resultCode == RESULT_OK) {
                mMediaProjection = mMediaProjectionManager.getMediaProjection(resultCode, intent);
                startRecording(); // defined below
            } else {
                // user did not grant permissions
            }
        }
    }

    /** It will start recording videos
     */
    private void startRecording() {
        DisplayManager dm = (DisplayManager)getSystemService(Context.DISPLAY_SERVICE);
        Display defaultDisplay = dm.getDisplay(Display.DEFAULT_DISPLAY);
        if (defaultDisplay == null) {
            throw new RuntimeException("No display found.");
        }
        prepareVideoEncoder();

        String videoPath= ("/storage/emulated/0/Movies/capture_"
                + new SimpleDateFormat("yyyy-MM-dd_HH-mm-ss").format(new Date()) + ".mp4");
        try {
            mMuxer = new MediaMuxer(videoPath, MediaMuxer.OutputFormat.MUXER_OUTPUT_MPEG_4);
        } catch (IOException ioe) {
            throw new RuntimeException("MediaMuxer creation failed", ioe);
        }

        // Get the display size and density.
        DisplayMetrics metrics = getResources().getDisplayMetrics();
        int screenWidth = metrics.widthPixels;
        int screenHeight = metrics.heightPixels;
        int screenDensity = metrics.densityDpi;

        // Start the video input.
        mMediaProjection.createVirtualDisplay("Recording Display", screenWidth,
                screenHeight, screenDensity, 0 /* flags */, mInputSurface,
                null /* callback */, null /* handler */);

        // Start the encoders
        drainEncoder();
    }

    /** Preparation of Video Encoder
     */
    private void prepareVideoEncoder() {
        mVideoBufferInfo = new MediaCodec.BufferInfo();
        MediaFormat format = MediaFormat.createVideoFormat(VIDEO_MIME_TYPE, VIDEO_WIDTH, VIDEO_HEIGHT);
        int frameRate = 30; // 30 fps

        /* Set required properties. */
        format.setInteger(MediaFormat.KEY_COLOR_FORMAT,
                MediaCodecInfo.CodecCapabilities.COLOR_FormatSurface);
        format.setInteger(MediaFormat.KEY_BIT_RATE, 6000000); // 6Mbps
        format.setInteger(MediaFormat.KEY_FRAME_RATE, frameRate);
        format.setInteger(MediaFormat.KEY_CAPTURE_RATE, frameRate);
        format.setInteger(MediaFormat.KEY_REPEAT_PREVIOUS_FRAME_AFTER, 1000000 / frameRate);
        format.setInteger(MediaFormat.KEY_CHANNEL_COUNT, 1);
        format.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, 1); // 1 seconds between I-frames

        /* Create a MediaCodec encoder and configure it. */
        try {
            mVideoEncoder = MediaCodec.createEncoderByType(VIDEO_MIME_TYPE);
            mVideoEncoder.configure(format, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
            mInputSurface = mVideoEncoder.createInputSurface();
            mVideoEncoder.start();
        } catch (IOException e) {
            releaseEncoders();
        }
    }

    /** It can start video recording and save the video
     * @return boolean
     */
    private boolean drainEncoder() {
        mDrainHandler.removeCallbacks(mDrainEncoderRunnable);
        while (true) {
            int bufferIndex = mVideoEncoder.dequeueOutputBuffer(mVideoBufferInfo, 0);

            if (bufferIndex == MediaCodec.INFO_TRY_AGAIN_LATER) {
                break;
            } else if (bufferIndex == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                if (mTrackIndex >= 0) {
                    throw new RuntimeException("format changed twice");
                }
                mTrackIndex = mMuxer.addTrack(mVideoEncoder.getOutputFormat());
                if (!mMuxerStarted && mTrackIndex >= 0) {
                    mMuxer.start();
                    mMuxerStarted = true;
                }
            } else if (bufferIndex < 0) {
                /* ignore */
            } else {
                ByteBuffer encodedData = mVideoEncoder.getOutputBuffer(bufferIndex);
                if (encodedData == null) {
                    throw new RuntimeException("couldn't fetch buffer at index " + bufferIndex);
                }

                if ((mVideoBufferInfo.flags & MediaCodec.BUFFER_FLAG_CODEC_CONFIG) != 0) {
                    mVideoBufferInfo.size = 0;
                }

                if (mVideoBufferInfo.size != 0) {
                    if (mMuxerStarted) {
                        encodedData.position(mVideoBufferInfo.offset);
                        encodedData.limit(mVideoBufferInfo.offset + mVideoBufferInfo.size);
                        mMuxer.writeSampleData(mTrackIndex, encodedData, mVideoBufferInfo);
                    } else {
                        /* Muxer not started */
                    }
                }

                mVideoEncoder.releaseOutputBuffer(bufferIndex, false);

                if ((mVideoBufferInfo.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) {
                    break;
                }
            }
        }

        mDrainHandler.postDelayed(mDrainEncoderRunnable, 10);
        return false;
    }


    /** It stops the video recording
     */
    private void releaseEncoders() {
        mDrainHandler.removeCallbacks(mDrainEncoderRunnable);
        if (mMuxer != null) {
            if (mMuxerStarted) {
                mMuxer.stop();
            }
            mMuxer.release();
            mMuxer = null;
            mMuxerStarted = false;
        }
        if (mVideoEncoder != null) {
            mVideoEncoder.stop();
            mVideoEncoder.release();
            mVideoEncoder = null;
        }
        if (mInputSurface != null) {
            mInputSurface.release();
            mInputSurface = null;
        }
        if (mMediaProjection != null) {
            mMediaProjection.stop();
            mMediaProjection = null;
        }
        mVideoBufferInfo = null;
        mDrainEncoderRunnable = null;
        mTrackIndex = -1;
    }

    @Override
    public void onResume() {
        super.onResume();
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
    }

    /** It shows the camera capture
     * @param view View
     */
    public void play(View view) {
        if (!isPlaying) {
            mVideoView.setVideoPath(url);
        } else {
            mVideoView.pause();
            isPlaying = false;
            playButton.setBackgroundResource(R.drawable.play_transparent);
        }
    }

    /** Turn Right Button, this command turns the camera to the right side.
     * This thread will send the command per 100 ms */
    public void rightThread() {
        new Thread(new Runnable() {
            public void run() {
                while (isRightThreadOn) {

                    if(position < 117) position += 10;

                    d1 = (byte) position;
                    data = udpClientClass.intToByteArray(id,d1,d2);

                    updateDisplayedValues();

                    try {
                        udpClientClass.setSendData(data);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }

                    try {
                        udpClientClass.send(ipAddress, port);
                        Log.d("rightThread", "RightSend");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }

                    try {
                        Thread.sleep(100);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        }).start();
    }

    /** Turn Left Button, this command turns the camera to the left side.
     * This thread will send the command per 100 ms*/
    public void leftThread() {
        new Thread(new Runnable() {
            public void run() {

                while (isLeftThreadOn) {

                    if (position > -117) position -= 10;

                    d1 = (byte) position;
                    data = udpClientClass.intToByteArray(id, d1, d2);

                    updateDisplayedValues();

                    try {
                        udpClientClass.setSendData(data);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }

                    try {
                        udpClientClass.send(ipAddress, port);
                        Log.d("rightThread", "LeftSend");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }

                    try {
                        Thread.sleep(100);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        }).start();
    }

    public void updateDisplayedValues() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                positionText.setText("Position: " + Integer.toString(position));
            }
        });
    }

    @Override
    public void onPause() {
        super.onPause();
        Log.d("Interface", "ConnectActivity onPause");
        Thread.interrupted();
    }

    /** Quit Button, by clicking on this button, user can quit the application.
     * @param view
     * @throws IOException
     */
    public void quit(View view) throws IOException {
        udpClientClass.closeSocket();
        moveTaskToBack(true);
        android.os.Process.killProcess(android.os.Process.myPid());
    }

    /** onDestroy, when this activity is destroyed, it will close the udp socket.*/
     @Override
    public void onDestroy() {
        super.onDestroy();
        try {
            udpClientClass.closeSocket();
        } catch (IOException e) {
            e.printStackTrace();
        }
        Thread.interrupted();
        finish();
    }

    /** Back Button, by pressing the back button, user can close the application
     */
    @Override
    public void onBackPressed() {
        super.onBackPressed();
        try {
            udpClientClass.closeSocket();
        } catch (IOException e) {
            e.printStackTrace();
        }
        moveTaskToBack(true);
        android.os.Process.killProcess(android.os.Process.myPid());
    }
}
