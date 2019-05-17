/*
 * Copyright (c) 2016 The CyanogenMod Project
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

package org.lineageos.pocketmode;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.IBinder;
import android.util.Log;

import java.util.ArrayList;
import java.util.List;

public class PocketModeService extends Service {
    private static final String TAG = "PocketModeService";
    private static final boolean DEBUG = false;

    private static final String ACTION_POCKETMODE_UPDATE = "org.lineageos.pocketmode.UPDATE";
    private static List<BroadcastReceiver> mReceiverList = new ArrayList<BroadcastReceiver>();

    private PocketSensor mPocketSensor;

    @Override
    public void onCreate() {
        if (DEBUG) Log.d(TAG, "Creating service");
        mPocketSensor = new PocketSensor(this);

        IntentFilter updateFilter = new IntentFilter(ACTION_POCKETMODE_UPDATE);
        registerReceiver(mUpdateReceiver, updateFilter);
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if (DEBUG) Log.d(TAG, "Starting service");
        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        if (DEBUG) Log.d(TAG, "Destroying service");
        super.onDestroy();
        if (mReceiverList.contains(mScreenStateReceiver)) {
            this.unregisterReceiver(mScreenStateReceiver);
        }
        this.unregisterReceiver(mUpdateReceiver);
        mPocketSensor.disable();
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    private void onDeviceUnlocked() {
        if (DEBUG) Log.d(TAG, "Device unlocked");
        mPocketSensor.disable();
    }

    private void onDisplayOff() {
        if (DEBUG) Log.d(TAG, "Display off");
        mPocketSensor.enable();
    }

    private BroadcastReceiver mScreenStateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(Intent.ACTION_USER_PRESENT)) {
                onDeviceUnlocked();
            } else if (intent.getAction().equals(Intent.ACTION_SCREEN_OFF)) {
                onDisplayOff();
            }
        }
    };

    private BroadcastReceiver mUpdateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getBooleanExtra("enable", false)) {
                IntentFilter screenStateFilter = new IntentFilter();
                screenStateFilter.addAction(Intent.ACTION_SCREEN_OFF);
                screenStateFilter.addAction(Intent.ACTION_USER_PRESENT);
                registerReceiver(mScreenStateReceiver, screenStateFilter);
                mReceiverList.add(mScreenStateReceiver);
            } else if (mReceiverList.contains(mScreenStateReceiver)) {
                unregisterReceiver(mScreenStateReceiver);
                mReceiverList.remove(mScreenStateReceiver);
                mPocketSensor.disable();
            }
        }
    };
}
