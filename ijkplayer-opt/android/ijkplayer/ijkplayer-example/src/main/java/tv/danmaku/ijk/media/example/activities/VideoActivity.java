/*
 * Copyright (C) 2015 Bilibili
 * Copyright (C) 2015 Zhang Rui <bbcallen@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package tv.danmaku.ijk.media.example.activities;

import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentTransaction;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.text.TextUtils;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.Spinner;
import android.widget.TableLayout;
import android.widget.TextView;
import android.widget.Toast;

import tv.danmaku.ijk.media.example.eventbus.PermissionHelper;
import tv.danmaku.ijk.media.player.IjkMediaPlayer;
import tv.danmaku.ijk.media.player.misc.ITrackInfo;
import tv.danmaku.ijk.media.example.R;
import tv.danmaku.ijk.media.example.application.Settings;
import tv.danmaku.ijk.media.example.content.RecentMediaStorage;
import tv.danmaku.ijk.media.example.fragments.TracksFragment;
import tv.danmaku.ijk.media.example.widget.media.AndroidMediaController;
import tv.danmaku.ijk.media.example.widget.media.IjkVideoView;
import tv.danmaku.ijk.media.example.widget.media.MeasureHelper;

public class VideoActivity extends AppCompatActivity implements TracksFragment.ITrackHolder,
        PermissionHelper.OnApplyPermissionListener {
    private static final String TAG = "VideoActivity";
    private static final int MSG_TYPE_RECORDING = 0x200;
    private String mVideoPath;
    private Uri    mVideoUri;

    private AndroidMediaController mMediaController;
    private IjkVideoView mVideoView;
    private TextView mToastTextView;
    private TableLayout mHudView;
    private DrawerLayout mDrawerLayout;
    private ViewGroup mRightDrawer;
    private TextView mRTimeTextView;
    private Spinner mFilterSpinner;

    private Spinner mFTypeSpinner;
    private Spinner mFRatioSpinner;
    private Spinner mFColorSpinner;
    private Spinner mScaleSpinner;
    private Spinner mFOutSpinner;
    private Spinner mLineWSpinner;
    private Spinner mAZoomSpinner;

    private Settings mSettings;
    private boolean mBackPressed;
    private long mRecordStartTime;
    private PermissionHelper mPermissionHelper;

    public static Intent newIntent(Context context, String videoPath, String videoTitle) {
        Intent intent = new Intent(context, VideoActivity.class);
        intent.putExtra("videoPath", videoPath);
        intent.putExtra("videoTitle", videoTitle);
        return intent;
    }

    public static void intentTo(Context context, String videoPath, String videoTitle) {
        context.startActivity(newIntent(context, videoPath, videoTitle));
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_player);

        mSettings = new Settings(this);
        mPermissionHelper = new PermissionHelper(this);
        // handle arguments
        mVideoPath = getIntent().getStringExtra("videoPath");

        Intent intent = getIntent();
        String intentAction = intent.getAction();
        if (!TextUtils.isEmpty(intentAction)) {
            if (intentAction.equals(Intent.ACTION_VIEW)) {
                mVideoPath = intent.getDataString();
            } else if (intentAction.equals(Intent.ACTION_SEND)) {
                mVideoUri = intent.getParcelableExtra(Intent.EXTRA_STREAM);
                if (Build.VERSION.SDK_INT < Build.VERSION_CODES.ICE_CREAM_SANDWICH) {
                    String scheme = mVideoUri.getScheme();
                    if (TextUtils.isEmpty(scheme)) {
                        Log.e(TAG, "Null unknown scheme\n");
                        finish();
                        return;
                    }
                    if (scheme.equals(ContentResolver.SCHEME_ANDROID_RESOURCE)) {
                        mVideoPath = mVideoUri.getPath();
                    } else if (scheme.equals(ContentResolver.SCHEME_CONTENT)) {
                        Log.e(TAG, "Can not resolve content below Android-ICS\n");
                        finish();
                        return;
                    } else {
                        Log.e(TAG, "Unknown scheme " + scheme + "\n");
                        finish();
                        return;
                    }
                }
            }
        }

        if (!TextUtils.isEmpty(mVideoPath)) {
            new RecentMediaStorage(this).saveUrlAsync(mVideoPath);
        }

        // init UI
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        ActionBar actionBar = getSupportActionBar();
        mMediaController = new AndroidMediaController(this, false);
        mMediaController.setSupportActionBar(actionBar);

        findViewById(R.id.btn_op_camera).setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view) {
                if (mVideoView.isRecording()){
                    mVideoView.appStopRecord();
                }else{
                    if (Build.VERSION.SDK_INT >= 23) {
                        mPermissionHelper.setOnApplyPermissionListener(VideoActivity.this);
                        mPermissionHelper.applyPermissions(PermissionHelper.WRITE_EXTERNAL_STORAGE_CODE);
                    }else{
                        startRecord();
                    }
                }
            }
        });
        int vType = IjkMediaPlayer.IJK_PLAY_LIVE;
        if (mVideoPath.endsWith("mp4") || mVideoPath.endsWith("MP4")){
            findViewById(R.id.btn_op_camera).setVisibility(View.GONE);
            vType = IjkMediaPlayer.IJK_PLAY_PLAYBACK;
        }
        mRTimeTextView = (TextView) findViewById(R.id.record_time_view);
        mToastTextView = (TextView) findViewById(R.id.toast_text_view);
        mHudView = (TableLayout) findViewById(R.id.hud_view);
        mDrawerLayout = (DrawerLayout) findViewById(R.id.drawer_layout);
        mRightDrawer = (ViewGroup) findViewById(R.id.right_drawer);

        mFilterSpinner = (Spinner) findViewById(R.id.filter_spinner);
        mFTypeSpinner = (Spinner)findViewById(R.id.ftype_spinner);
        mFRatioSpinner = (Spinner)findViewById(R.id.fratio_spinner);
        mFColorSpinner = (Spinner)findViewById(R.id.fcolor_spinner);
        mScaleSpinner = (Spinner)findViewById(R.id.scale_spinner);
        mFOutSpinner = (Spinner)findViewById(R.id.fout_spinner);
        mLineWSpinner = (Spinner)findViewById(R.id.flinew_spinner);
        mAZoomSpinner = (Spinner)findViewById(R.id.azoom_spinner);

        mFilterSpinner.setOnItemSelectedListener(spinnerListner);
        mFTypeSpinner.setOnItemSelectedListener(spinnerListner);
        mFRatioSpinner.setOnItemSelectedListener(spinnerListner);
        mFColorSpinner.setOnItemSelectedListener(spinnerListner);
        mScaleSpinner.setOnItemSelectedListener(spinnerListner);
        mFOutSpinner.setOnItemSelectedListener(spinnerListner);
        mAZoomSpinner.setOnItemSelectedListener(spinnerListner);
        mLineWSpinner.setOnItemSelectedListener(spinnerListner);

        mDrawerLayout.setScrimColor(Color.TRANSPARENT);

        // init player
        IjkMediaPlayer.loadLibrariesOnce(null);
        IjkMediaPlayer.native_profileBegin("libijkplayer.so");

        mVideoView = (IjkVideoView) findViewById(R.id.video_view);
        mVideoView.setMediaController(mMediaController);
        mVideoView.setHudView(mHudView);
        mHudView.setVisibility(android.view.View.GONE);
        // prefer mVideoPath
        if (mVideoPath != null)
            mVideoView.setVideoPath(mVideoPath,vType);
        else if (mVideoUri != null)
            mVideoView.setVideoURI(mVideoUri,vType);
        else {
            Log.e(TAG, "Null Data Source\n");
            finish();
            return;
        }
        mVideoView.start();
    }
    private static int colors[] = {0xFFFF0000,0xFFFFFF00,
            0xFFFF6100,0xFF00FF00,0xFF0000FF,0xFFFF0000,0xFF082E54};
    private static float ratios[] = {1.0f,4.0f/3.0f,13.0f/9,14.0f/9,16.0f/9,2.0f};
    private static int fcolors[] = {0xFFFF0000,0xFF00FF00,0xFF0000FF,0xFFFFFFFF,0xFFFFFF00};
    private static int fOutAlpha[] = {100,75,50,25,0};
    private static int fLineW[] = {2,4,6,8,10};
    private static int fAZoom[] = {0x0A,0x1E,0x3C,0x5A,0x64};
    private static int fAuxFocusSensity[] = {0x0A,0x1E,0x32,0x4B,0x5A};
    private static int framecmd[] = {0xF0,0x10,0x20,0x100,0x100,0x100,0x100,0x30,0x30,0x30,0x40,0x50,0x60};
    private static float fScale[] = {2.0f,3.0f,4.0f};
    private static int filtercmd[] = {0xF,0x1,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x3,0x6,0x6,0x4,0x5,0x5,0x5};
    private Spinner.OnItemSelectedListener spinnerListner =
            new Spinner.OnItemSelectedListener() {
        @Override
        public void onItemSelected(AdapterView<?> arg0, View arg1,
            int arg2, long arg3) {
            int fidxType = arg0 == mFilterSpinner ? arg2 : mFilterSpinner.getSelectedItemPosition();
            int idxType = arg0 == mFTypeSpinner ? arg2 : mFTypeSpinner.getSelectedItemPosition();
            int idxRatio = arg0 == mFRatioSpinner ? arg2 : mFRatioSpinner.getSelectedItemPosition();
            int idxColor = arg0 == mFColorSpinner ? arg2 : mFColorSpinner.getSelectedItemPosition();
            int idxScale = arg0 == mScaleSpinner ? arg2 : mScaleSpinner.getSelectedItemPosition();
            int idxFout = arg0 == mFOutSpinner ? arg2 : mFOutSpinner.getSelectedItemPosition();
            int idxFLineW = arg0 == mLineWSpinner ? arg2 : mLineWSpinner.getSelectedItemPosition();
            int idxAZoom = arg0 == mAZoomSpinner ? arg2 : mAZoomSpinner.getSelectedItemPosition();
            int cmd = framecmd[idxType];

            float rati = cmd == 0x30 ? fScale[idxScale] : (cmd == 0x40 || cmd == 0x50 ?
                    fOutAlpha[idxFout] : ratios[idxRatio]);
            int centerX = idxType == framecmd.length - 4 ? 75 : (idxType == framecmd.length - 6 ? 25 : 50);
            int centerY = 50;
            int type = cmd == 0x10 ? fAZoom[idxAZoom] | (fOutAlpha[idxFout] << 16) : (
                    cmd == 0x100 ? idxType - 3: (cmd == 0x30 ? idxType - 6 : fOutAlpha[idxFout]));
            mVideoView.setEGLFilter(cmd,type, centerX, centerY, rati, fcolors[idxColor],
                    fLineW[idxFLineW], "");

            cmd = filtercmd[fidxType];
            rati = cmd == 0x6 ? (fidxType == 10 ? 50.0f : 80.0f) : fAuxFocusSensity[idxAZoom];
            mVideoView.setEGLFilter(cmd, 0x0, 0, 0,
                    rati, cmd  == 0x2 ? colors[fidxType - 2] : fcolors[idxColor], fLineW[idxFLineW],
                    cmd == 0x5 ? "/sdcard/EA_Cinematic_Lut" + (filtercmd.length - fidxType) + ".cube" : "");
        }
        @Override
        public void onNothingSelected(AdapterView<?> arg0) {
        }
    };
    private void startRecord(){
       mVideoView.appStartRecord("/sdcard/DCIM/Camera/test.mp4", 30);
       mHandler.sendEmptyMessageDelayed(MSG_TYPE_RECORDING, 2000);
       mRecordStartTime = System.currentTimeMillis() / 1000;
    }

    private void cameraNotify(){
        Uri localUri =  Uri.parse("/sdcard/DCIM/Camera/test.mp4");
        Intent localIntent = new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE, localUri);
        sendBroadcast(localIntent);
    }
    @Override
    public void onBackPressed() {
        mBackPressed = true;

        super.onBackPressed();
    }
    @Override
    protected void onPause(){
        if (mVideoView.isRecording()){
            mVideoView.appStopRecord();
        }
        super.onPause();
    }

    @Override
    protected void onStop() {
        super.onStop();

        if (mBackPressed || !mVideoView.isBackgroundPlayEnabled()) {
            mVideoView.stopPlayback();
            mVideoView.release(true);
            mVideoView.stopBackgroundPlay();
        } else {
            mVideoView.enterBackground();
        }
        IjkMediaPlayer.native_profileEnd();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_player, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        if (id == R.id.action_toggle_ratio) {
            int aspectRatio = mVideoView.toggleAspectRatio();
            String aspectRatioText = MeasureHelper.getAspectRatioText(this, aspectRatio);
            mToastTextView.setText(aspectRatioText);
            mMediaController.showOnce(mToastTextView);
            return true;
        } else if (id == R.id.action_toggle_player) {
            int player = mVideoView.togglePlayer();
            String playerText = IjkVideoView.getPlayerText(this, player);
            mToastTextView.setText(playerText);
            mMediaController.showOnce(mToastTextView);
            return true;
        } else if (id == R.id.action_toggle_render) {
            int render = mVideoView.toggleRender();
            String renderText = IjkVideoView.getRenderText(this, render);
            mToastTextView.setText(renderText);
            mMediaController.showOnce(mToastTextView);
            return true;
        } else if (id == R.id.action_show_info) {
            mVideoView.showMediaInfo();
        } else if (id == R.id.action_show_tracks) {
            if (mDrawerLayout.isDrawerOpen(mRightDrawer)) {
                Fragment f = getSupportFragmentManager().findFragmentById(R.id.right_drawer);
                if (f != null) {
                    FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
                    transaction.remove(f);
                    transaction.commit();
                }
                mDrawerLayout.closeDrawer(mRightDrawer);
            } else {
                Fragment f = TracksFragment.newInstance();
                FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
                transaction.replace(R.id.right_drawer, f);
                transaction.commit();
                mDrawerLayout.openDrawer(mRightDrawer);
            }
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        mPermissionHelper.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }
    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        mPermissionHelper.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    public void onAfterApplyAllPermission(int code) {
        if (code == PermissionHelper.WRITE_EXTERNAL_STORAGE_CODE){
            startRecord();
        }
    }
    @Override
    public ITrackInfo[] getTrackInfo() {
        if (mVideoView == null)
            return null;

        return mVideoView.getTrackInfo();
    }

    @Override
    public void selectTrack(int stream) {
        mVideoView.selectTrack(stream);
    }

    @Override
    public void deselectTrack(int stream) {
        mVideoView.deselectTrack(stream);
    }

    @Override
    public int getSelectedTrack(int trackType) {
        if (mVideoView == null)
            return -1;

        return mVideoView.getSelectedTrack(trackType);
    }

    private Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case MSG_TYPE_RECORDING:
                    if (mVideoView.isRecording()){
                        long time = System.currentTimeMillis() / 1000 - mRecordStartTime;
                        mRTimeTextView.setVisibility(View.VISIBLE);
                        mRTimeTextView.setText(secToTime(time));
                        mHandler.sendEmptyMessageDelayed(MSG_TYPE_RECORDING,1000);
                    }else{
                        mRTimeTextView.setVisibility(View.INVISIBLE);
                        Toast.makeText(VideoActivity.this, "已保存到相册",
                                Toast.LENGTH_LONG).show();
                        cameraNotify();
                    }
                    break;
            }
        }
    };

    private static String secToTime(long sec) {
        int h = 0;
        int d = 0;
        int s = 0;
        int second = (int) sec;
        int temp = second % 3600;
        if (second > 3600) {
            h = second / 3600;
            if (temp != 0) {
                if (temp > 60) {
                    d = temp / 60;
                    if (temp % 60 != 0) {
                        s = temp % 60;
                    }
                } else {
                    s = temp;
                }
            }
        } else {
            d = second / 60;
            if (second % 60 != 0) {
                s = second % 60;
            }
        }
        return String.format("%0" + 2 + "d", h) + ":"
                + String.format("%0" + 2 + "d", d) + ":" + String.format("%0" + 2 + "d", s);
    }
}
