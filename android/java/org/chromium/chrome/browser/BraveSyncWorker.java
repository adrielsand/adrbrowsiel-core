/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser;

import android.content.Context;
import android.content.SharedPreferences;

import org.chromium.base.ContextUtils;
import org.chromium.base.Log;
import org.chromium.base.ThreadUtils;
import org.chromium.base.annotations.CalledByNative;
import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;
import org.chromium.base.task.PostTask;
import org.chromium.base.task.TaskTraits;

import java.lang.Runnable;

@JNINamespace("chrome::android")
public class adrbrowsielSyncWorker {
    public static final String TAG = "SYNC";

    private Context mContext;
    private String mDebug = "true";

    private long mNativeadrbrowsielSyncWorker;

    @CalledByNative
    private void setNativePtr(long nativePtr) {
        assert mNativeadrbrowsielSyncWorker == 0;
        mNativeadrbrowsielSyncWorker = nativePtr;
    }

    private void Init() {
        if (mNativeadrbrowsielSyncWorker == 0) {
            adrbrowsielSyncWorkerJni.get().init(adrbrowsielSyncWorker.this);
        }
    }

    @Override
    protected void finalize() {
        Destroy();
    }

    private void Destroy() {
        if (mNativeadrbrowsielSyncWorker != 0) {
            adrbrowsielSyncWorkerJni.get().destroy(mNativeadrbrowsielSyncWorker);
            mNativeadrbrowsielSyncWorker = 0;
        }
    }

    public adrbrowsielSyncWorker() {
        mContext = ContextUtils.getApplicationContext();
        Init();
        (new MigrationFromV1()).MigrateFromSyncV1();
    }

    private class MigrationFromV1 {
        // Deprecated
        public static final String PREF_NAME = "SyncPreferences";
        private static final String PREF_LAST_FETCH_NAME = "TimeLastFetch";
        private static final String PREF_LATEST_DEVICE_RECORD_TIMESTAMPT_NAME =
                "LatestDeviceRecordTime";
        private static final String PREF_LAST_TIME_SEND_NOT_SYNCED_NAME = "TimeLastSendNotSynced";
        public static final String PREF_DEVICE_ID = "DeviceId";
        public static final String PREF_BASE_ORDER = "BaseOrder";
        public static final String PREF_LAST_ORDER = "LastOrder";
        public static final String PREF_SEED = "Seed";
        public static final String PREF_SYNC_DEVICE_NAME = "SyncDeviceName";
        private static final String PREF_SYNC_SWITCH = "sync_switch";
        private static final String PREF_SYNC_BOOKMARKS = "adrbrowsiel_sync_bookmarks";
        public static final String PREF_SYNC_TABS = "adrbrowsiel_sync_tabs"; // never used
        public static final String PREF_SYNC_HISTORY = "adrbrowsiel_sync_history"; // never used
        public static final String PREF_SYNC_AUTOFILL_PASSWORDS =
                "adrbrowsiel_sync_autofill_passwords"; // never used
        public static final String PREF_SYNC_PAYMENT_SETTINGS =
                "adrbrowsiel_sync_payment_settings"; // never used

        private boolean HaveSyncV1Prefs() {
            SharedPreferences sharedPref = mContext.getSharedPreferences(PREF_NAME, 0);

            String deviceId = sharedPref.getString(PREF_DEVICE_ID, null);
            if (null == deviceId) {
                return false;
            }
            return true;
        }

        private void DeleteSyncV1Prefs() {
            SharedPreferences sharedPref = mContext.getSharedPreferences(PREF_NAME, 0);
            SharedPreferences.Editor editor = sharedPref.edit();
            editor.clear().apply();
        }

        private void DeleteSyncV1LevelDb() {
            adrbrowsielSyncWorkerJni.get().destroyV1LevelDb();
        }

        public void MigrateFromSyncV1() {
            // Do all migration work in file IO thread because we may need to
            // read shared preferences and delete level db
            PostTask.postTask(TaskTraits.BEST_EFFORT_MAY_BLOCK, () -> {
                if (HaveSyncV1Prefs()) {
                    Log.i(TAG, "Found sync v1 data, doing migration");
                    DeleteSyncV1Prefs();
                    DeleteSyncV1LevelDb();
                    // Mark sync v1 was enabled to trigger informers
                    ThreadUtils.runOnUiThreadBlocking(new Runnable() {
                        @Override
                        public void run() {
                            adrbrowsielSyncWorkerJni.get().markSyncV1WasEnabledAndMigrated();
                            adrbrowsielSyncInformers.show();
                        }
                    });
                }
            });
        }
    };

    public String GetCodephrase() {
        return adrbrowsielSyncWorkerJni.get().getSyncCodeWords(mNativeadrbrowsielSyncWorker);
    }

    public void SaveCodephrase(String codephrase) {
        adrbrowsielSyncWorkerJni.get().saveCodeWords(mNativeadrbrowsielSyncWorker, codephrase);
    }

    public String GetSeedHexFromWords(String codephrase) {
        return adrbrowsielSyncWorkerJni.get().getSeedHexFromWords(codephrase);
    }

    public String GetWordsFromSeedHex(String seedHex) {
        return adrbrowsielSyncWorkerJni.get().getWordsFromSeedHex(seedHex);
    }

    public void RequestSync() {
        adrbrowsielSyncWorkerJni.get().requestSync(mNativeadrbrowsielSyncWorker);
    }

    public boolean IsFirstSetupComplete() {
        return adrbrowsielSyncWorkerJni.get().isFirstSetupComplete(mNativeadrbrowsielSyncWorker);
    }

    public void FinalizeSyncSetup() {
        adrbrowsielSyncWorkerJni.get().finalizeSyncSetup(mNativeadrbrowsielSyncWorker);
    }

    public void ResetSync() {
        adrbrowsielSyncWorkerJni.get().resetSync(mNativeadrbrowsielSyncWorker);
    }

    public boolean getSyncV1WasEnabled() {
        return adrbrowsielSyncWorkerJni.get().getSyncV1WasEnabled(mNativeadrbrowsielSyncWorker);
    }

    public boolean getSyncV2MigrateNoticeDismissed() {
        return adrbrowsielSyncWorkerJni.get().getSyncV2MigrateNoticeDismissed(mNativeadrbrowsielSyncWorker);
    }

    public void setSyncV2MigrateNoticeDismissed(boolean isDismissed) {
        adrbrowsielSyncWorkerJni.get().setSyncV2MigrateNoticeDismissed(
                mNativeadrbrowsielSyncWorker, isDismissed);
    }

    @NativeMethods
    interface Natives {
        void init(adrbrowsielSyncWorker caller);
        void destroy(long nativeadrbrowsielSyncWorker);

        void destroyV1LevelDb();
        void markSyncV1WasEnabledAndMigrated();

        String getSyncCodeWords(long nativeadrbrowsielSyncWorker);
        void requestSync(long nativeadrbrowsielSyncWorker);

        String getSeedHexFromWords(String passphrase);
        String getWordsFromSeedHex(String seedHex);
        void saveCodeWords(long nativeadrbrowsielSyncWorker, String passphrase);

        void finalizeSyncSetup(long nativeadrbrowsielSyncWorker);

        boolean isFirstSetupComplete(long nativeadrbrowsielSyncWorker);

        void resetSync(long nativeadrbrowsielSyncWorker);

        boolean getSyncV1WasEnabled(long nativeadrbrowsielSyncWorker);
        boolean getSyncV2MigrateNoticeDismissed(long nativeadrbrowsielSyncWorker);
        void setSyncV2MigrateNoticeDismissed(long nativeadrbrowsielSyncWorker, boolean isDismissed);
    }
}
