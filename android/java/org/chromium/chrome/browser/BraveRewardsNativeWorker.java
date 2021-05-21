/** Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v. 2.0. If a copy of the MPL was not distributed with this file,
  * You can obtain one at http://mozilla.org/MPL/2.0/.
  */

package org.chromium.chrome.browser;

import android.os.Handler;

import androidx.annotation.Nullable;

import org.json.JSONException;

import org.chromium.base.Log;
import org.chromium.base.annotations.CalledByNative;
import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;
import org.chromium.chrome.browser.adrbrowsielRewardsBalance;
import org.chromium.chrome.browser.adrbrowsielRewardsHelper;
import org.chromium.chrome.browser.adrbrowsielRewardsObserver;
import org.chromium.chrome.browser.adrbrowsielRewardsPublisher.PublisherStatus;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.components.embedder_support.util.UrlConstants;

import java.util.ArrayList;
import java.util.List;

@JNINamespace("chrome::android")
public class adrbrowsielRewardsNativeWorker {
    /**
     * Allows to monitor a front tab publisher changes.
     */
    public interface PublisherObserver { void onFrontTabPublisherChanged(boolean verified); }

    // Rewards notifications
    // Taken from components/adrbrowsiel_rewards/browser/rewards_notification_service.h
    public static final int REWARDS_NOTIFICATION_INVALID = 0;
    public static final int REWARDS_NOTIFICATION_AUTO_CONTRIBUTE = 1;
    public static final int REWARDS_NOTIFICATION_GRANT = 2;
    public static final int REWARDS_NOTIFICATION_GRANT_ADS = 3;
    public static final int REWARDS_NOTIFICATION_FAILED_CONTRIBUTION = 4;
    public static final int REWARDS_NOTIFICATION_IMPENDING_CONTRIBUTION = 5;
    public static final int REWARDS_NOTIFICATION_INSUFFICIENT_FUNDS = 6;
    public static final int REWARDS_NOTIFICATION_BACKUP_WALLET = 7;
    public static final int REWARDS_NOTIFICATION_TIPS_PROCESSED = 8;
    public static final int REWARDS_NOTIFICATION_ADS_ONBOARDING = 9;
    public static final int REWARDS_NOTIFICATION_VERIFIED_PUBLISHER = 10;

    public static final int LEDGER_OK = 0;
    public static final int LEDGER_ERROR = 1;
    public static final int WALLET_CREATED = 12;
    public static final int BAT_NOT_ALLOWED = 25;
    public static final int SAFETYNET_ATTESTATION_FAILED = 27;

    private static final int REWARDS_UNKNOWN = 0;
    private static final int REWARDS_DISABLED = 1;
    private static final int REWARDS_ENABLED = 2;
    private static int rewardsStatus = REWARDS_UNKNOWN;
    private String frontTabUrl;
    private static final Handler mHandler = new Handler();

    private List<adrbrowsielRewardsObserver> mObservers;
    private List<PublisherObserver> mFrontTabPublisherObservers;
    private long mNativeadrbrowsielRewardsNativeWorker;

    private static adrbrowsielRewardsNativeWorker instance;
    private static final Object lock = new Object();
    private boolean grantClaimInProcess;  // flag: wallet is being created

    public static  adrbrowsielRewardsNativeWorker getInstance() {
        synchronized(lock) {
          if(instance == null) {
              instance = new adrbrowsielRewardsNativeWorker();
              instance.Init();
          }
        }
        return instance;
    }

    private adrbrowsielRewardsNativeWorker() {
        mObservers = new ArrayList<adrbrowsielRewardsObserver>();
        mFrontTabPublisherObservers = new ArrayList<PublisherObserver>();
    }

    private void Init() {
      if (mNativeadrbrowsielRewardsNativeWorker == 0) {
          adrbrowsielRewardsNativeWorkerJni.get().init(adrbrowsielRewardsNativeWorker.this);
      }
    }

    @Override
    protected void finalize() {
        Destroy();
    }

    private void Destroy() {
        if (mNativeadrbrowsielRewardsNativeWorker != 0) {
            adrbrowsielRewardsNativeWorkerJni.get().destroy(mNativeadrbrowsielRewardsNativeWorker);
            mNativeadrbrowsielRewardsNativeWorker = 0;
        }
    }

    public void AddObserver(adrbrowsielRewardsObserver observer) {
        synchronized(lock) {
            mObservers.add(observer);
        }
    }

    public void RemoveObserver(adrbrowsielRewardsObserver observer) {
        synchronized(lock) {
            mObservers.remove(observer);
        }
    }

    public void AddPublisherObserver(PublisherObserver observer) {
        synchronized (lock) {
            mFrontTabPublisherObservers.add(observer);
        }
    }

    public void RemovePublisherObserver(PublisherObserver observer) {
        synchronized (lock) {
            mFrontTabPublisherObservers.remove(observer);
        }
    }

    public void OnNotifyFrontTabUrlChanged(int tabId, String url) {
        boolean chromeUrl = url.startsWith(UrlConstants.CHROME_SCHEME);
        boolean newUrl = (frontTabUrl == null || !frontTabUrl.equals(url));
        if (chromeUrl) {
            // Don't query 'GetPublisherInfo' and post response now.
            mHandler.post(new Runnable() {
                @Override
                public void run() {
                    NotifyPublisherObservers(false);
                }
            });
        } else if (newUrl) {
            GetPublisherInfo(tabId, url);
        }

        frontTabUrl = url;
    }

    private void NotifyPublisherObservers(boolean verified) {
        for (PublisherObserver observer : mFrontTabPublisherObservers) {
            observer.onFrontTabPublisherChanged(verified);
        }
    }

    public void TriggerOnNotifyFrontTabUrlChanged() {
        // Clear frontTabUrl so that all observers are updated.
        frontTabUrl = "";
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                Tab tab = adrbrowsielRewardsHelper.currentActiveChromeTabbedActivityTab();
                if (tab != null && !tab.isIncognito()) {
                    OnNotifyFrontTabUrlChanged(tab.getId(), tab.getUrlString());
                }
            }
        });
    }

    public boolean IsGrantClaimInProcess() {
        synchronized(lock) {
          return grantClaimInProcess;
        }
    }

    public void GetRewardsParameters() {
        synchronized(lock) {
            adrbrowsielRewardsNativeWorkerJni.get().getRewardsParameters(mNativeadrbrowsielRewardsNativeWorker);
        }
    }

    @Nullable
    public adrbrowsielRewardsBalance GetWalletBalance() {
        synchronized(lock) {
            String json = adrbrowsielRewardsNativeWorkerJni.get().getWalletBalance(
                    mNativeadrbrowsielRewardsNativeWorker);
            adrbrowsielRewardsBalance balance = null;
            try{
                balance = new adrbrowsielRewardsBalance (json);
            }
            catch (JSONException e) {
                balance = null;
            }
            return balance;
        }
    }

    public double GetWalletRate() {
        synchronized(lock) {
            return adrbrowsielRewardsNativeWorkerJni.get().getWalletRate(mNativeadrbrowsielRewardsNativeWorker);
        }
    }

    public void GetPublisherInfo(int tabId, String host) {
        synchronized(lock) {
            adrbrowsielRewardsNativeWorkerJni.get().getPublisherInfo(
                    mNativeadrbrowsielRewardsNativeWorker, tabId, host);
        }
    }

    public String GetPublisherURL(int tabId) {
        synchronized(lock) {
            return adrbrowsielRewardsNativeWorkerJni.get().getPublisherURL(
                    mNativeadrbrowsielRewardsNativeWorker, tabId);
        }
    }

    public String GetPublisherFavIconURL(int tabId) {
        synchronized(lock) {
            return adrbrowsielRewardsNativeWorkerJni.get().getPublisherFavIconURL(
                    mNativeadrbrowsielRewardsNativeWorker, tabId);
        }
    }

    public String GetPublisherName(int tabId) {
        synchronized(lock) {
            return adrbrowsielRewardsNativeWorkerJni.get().getPublisherName(
                    mNativeadrbrowsielRewardsNativeWorker, tabId);
        }
    }

    public String GetPublisherId(int tabId) {
        synchronized(lock) {
            return adrbrowsielRewardsNativeWorkerJni.get().getPublisherId(
                    mNativeadrbrowsielRewardsNativeWorker, tabId);
        }
    }

    public int GetPublisherPercent(int tabId) {
        synchronized(lock) {
            return adrbrowsielRewardsNativeWorkerJni.get().getPublisherPercent(
                    mNativeadrbrowsielRewardsNativeWorker, tabId);
        }
    }

    public boolean GetPublisherExcluded(int tabId) {
        synchronized(lock) {
            return adrbrowsielRewardsNativeWorkerJni.get().getPublisherExcluded(
                    mNativeadrbrowsielRewardsNativeWorker, tabId);
        }
    }

    public @PublisherStatus int GetPublisherStatus(int tabId) {
        synchronized(lock) {
            return adrbrowsielRewardsNativeWorkerJni.get().getPublisherStatus(
                    mNativeadrbrowsielRewardsNativeWorker, tabId);
        }
    }

    public void IncludeInAutoContribution(int tabId, boolean exclude) {
        synchronized(lock) {
            adrbrowsielRewardsNativeWorkerJni.get().includeInAutoContribution(
                    mNativeadrbrowsielRewardsNativeWorker, tabId, exclude);
        }
    }

    public void RemovePublisherFromMap(int tabId) {
        synchronized(lock) {
            adrbrowsielRewardsNativeWorkerJni.get().removePublisherFromMap(
                    mNativeadrbrowsielRewardsNativeWorker, tabId);
        }
    }

    public void GetCurrentBalanceReport() {
        synchronized(lock) {
            adrbrowsielRewardsNativeWorkerJni.get().getCurrentBalanceReport(
                    mNativeadrbrowsielRewardsNativeWorker);
        }
    }

    public void Donate(String publisher_key, int amount, boolean recurring) {
        synchronized(lock) {
            adrbrowsielRewardsNativeWorkerJni.get().donate(
                    mNativeadrbrowsielRewardsNativeWorker, publisher_key, amount, recurring);
        }
    }

    public void GetAllNotifications() {
        synchronized(lock) {
            adrbrowsielRewardsNativeWorkerJni.get().getAllNotifications(mNativeadrbrowsielRewardsNativeWorker);
        }
    }

    public void DeleteNotification(String notification_id) {
        synchronized(lock) {
            adrbrowsielRewardsNativeWorkerJni.get().deleteNotification(
                    mNativeadrbrowsielRewardsNativeWorker, notification_id);
        }
    }

    public void GetGrant(String promotionId) {
        synchronized(lock) {
            if (grantClaimInProcess) {
                return;
            }
            grantClaimInProcess = true;
            adrbrowsielRewardsNativeWorkerJni.get().getGrant(
                    mNativeadrbrowsielRewardsNativeWorker, promotionId);
        }
    }

    public String[] GetCurrentGrant(int position) {
        synchronized(lock) {
            return adrbrowsielRewardsNativeWorkerJni.get().getCurrentGrant(
                    mNativeadrbrowsielRewardsNativeWorker, position);
        }
    }

    public void GetPendingContributionsTotal() {
        synchronized(lock) {
            adrbrowsielRewardsNativeWorkerJni.get().getPendingContributionsTotal(
                    mNativeadrbrowsielRewardsNativeWorker);
        }
    }

    public void GetRecurringDonations() {
        synchronized(lock) {
            adrbrowsielRewardsNativeWorkerJni.get().getRecurringDonations(
                    mNativeadrbrowsielRewardsNativeWorker);
        }
    }

    public boolean IsCurrentPublisherInRecurrentDonations(String publisher) {
        synchronized(lock) {
            return adrbrowsielRewardsNativeWorkerJni.get().isCurrentPublisherInRecurrentDonations(
                    mNativeadrbrowsielRewardsNativeWorker, publisher);
        }
    }

    public double GetPublisherRecurrentDonationAmount(String publisher) {
        synchronized(lock) {
            return adrbrowsielRewardsNativeWorkerJni.get().getPublisherRecurrentDonationAmount(
                    mNativeadrbrowsielRewardsNativeWorker, publisher);
        }
    }

    public void GetAutoContributeProperties() {
        synchronized(lock) {
            adrbrowsielRewardsNativeWorkerJni.get().getAutoContributeProperties(
                    mNativeadrbrowsielRewardsNativeWorker);
        }
    }

    public boolean IsAutoContributeEnabled() {
        synchronized(lock) {
            return adrbrowsielRewardsNativeWorkerJni.get().isAutoContributeEnabled(
                    mNativeadrbrowsielRewardsNativeWorker);
        }
    }

    public void GetReconcileStamp() {
        synchronized(lock) {
            adrbrowsielRewardsNativeWorkerJni.get().getReconcileStamp(mNativeadrbrowsielRewardsNativeWorker);
        }
    }

    public void RemoveRecurring(String publisher) {
        synchronized(lock) {
            adrbrowsielRewardsNativeWorkerJni.get().removeRecurring(
                    mNativeadrbrowsielRewardsNativeWorker, publisher);
        }
    }

    public void ResetTheWholeState() {
        synchronized(lock) {
            adrbrowsielRewardsNativeWorkerJni.get().resetTheWholeState(mNativeadrbrowsielRewardsNativeWorker);
        }
    }

    public void FetchGrants() {
        synchronized(lock) {
            adrbrowsielRewardsNativeWorkerJni.get().fetchGrants(mNativeadrbrowsielRewardsNativeWorker);
        }
    }

    public int GetAdsPerHour() {
        synchronized (lock) {
            return adrbrowsielRewardsNativeWorkerJni.get().getAdsPerHour(mNativeadrbrowsielRewardsNativeWorker);
        }
    }

    public void SetAdsPerHour(int value) {
        synchronized (lock) {
            adrbrowsielRewardsNativeWorkerJni.get().setAdsPerHour(mNativeadrbrowsielRewardsNativeWorker, value);
        }
    }

    public boolean IsAnonWallet() {
        synchronized(lock) {
            return adrbrowsielRewardsNativeWorkerJni.get().isAnonWallet(mNativeadrbrowsielRewardsNativeWorker);
        }
    }

    public boolean isRewardsEnabled() {
        synchronized (lock) {
            return adrbrowsielRewardsNativeWorkerJni.get().isRewardsEnabled(
                    mNativeadrbrowsielRewardsNativeWorker);
        }
    }

    public void GetExternalWallet() {
        synchronized (lock) {
            adrbrowsielRewardsNativeWorkerJni.get().getExternalWallet(mNativeadrbrowsielRewardsNativeWorker);
        }
    }

    public void DisconnectWallet() {
        synchronized (lock) {
            adrbrowsielRewardsNativeWorkerJni.get().disconnectWallet(mNativeadrbrowsielRewardsNativeWorker);
        }
    }

    public void ProcessRewardsPageUrl(String path, String query) {
        synchronized (lock) {
            adrbrowsielRewardsNativeWorkerJni.get().processRewardsPageUrl(
                    mNativeadrbrowsielRewardsNativeWorker, path, query);
        }
    }

    public void RecoverWallet(String passPhrase) {
        synchronized (lock) {
            adrbrowsielRewardsNativeWorkerJni.get().recoverWallet(
                    mNativeadrbrowsielRewardsNativeWorker, passPhrase);
        }
    }

    @CalledByNative
    public void OnRecoverWallet(int errorCode) {
        for (adrbrowsielRewardsObserver observer : mObservers) {
            observer.OnRecoverWallet(errorCode);
        }
    }

    public void RefreshPublisher(String publisherKey) {
        synchronized (lock) {
            adrbrowsielRewardsNativeWorkerJni.get().refreshPublisher(
                    mNativeadrbrowsielRewardsNativeWorker, publisherKey);
        }
    }

    public void SetAutoContributeEnabled(boolean isSetAutoContributeEnabled) {
        synchronized(lock) {
            adrbrowsielRewardsNativeWorkerJni.get().setAutoContributeEnabled(
                    mNativeadrbrowsielRewardsNativeWorker, isSetAutoContributeEnabled);
        }
    }

    public void SetAutoContributionAmount(double amount) {
        synchronized(lock) {
            adrbrowsielRewardsNativeWorkerJni.get().setAutoContributionAmount(
                    mNativeadrbrowsielRewardsNativeWorker, amount);
        }
    }

    public void StartProcess() {
        synchronized (lock) {
            adrbrowsielRewardsNativeWorkerJni.get().startProcess(mNativeadrbrowsielRewardsNativeWorker);
        }
    }

    @CalledByNative
    public void OnStartProcess() {
        for (adrbrowsielRewardsObserver observer : mObservers) {
            observer.OnStartProcess();
        }
    }

    @CalledByNative
    public void OnRefreshPublisher(int status, String publisherKey) {
        for (adrbrowsielRewardsObserver observer : mObservers) {
            observer.OnRefreshPublisher(status, publisherKey);
        }
    }

    @CalledByNative
    public void OnRewardsParameters(int errorCode) {
        for (adrbrowsielRewardsObserver observer : mObservers) {
            observer.OnRewardsParameters(errorCode);
        }
    }

    @CalledByNative
    public void OnGetCurrentBalanceReport(double[] report) {
        for (adrbrowsielRewardsObserver observer : mObservers) {
            observer.OnGetCurrentBalanceReport(report);
        }
    }

    @CalledByNative
    private void setNativePtr(long nativePtr) {
        assert mNativeadrbrowsielRewardsNativeWorker == 0;
        mNativeadrbrowsielRewardsNativeWorker = nativePtr;
    }

    @CalledByNative
    public void OnPublisherInfo(int tabId) {
        @PublisherStatus int pubStatus = GetPublisherStatus(tabId);
        boolean verified = (pubStatus == adrbrowsielRewardsPublisher.CONNECTED
                                   || pubStatus == adrbrowsielRewardsPublisher.UPHOLD_VERIFIED)
                ? true
                : false;
        NotifyPublisherObservers(verified);

        // Notify adrbrowsielRewardsObserver (panel).
        for (adrbrowsielRewardsObserver observer : mObservers) {
            observer.OnPublisherInfo(tabId);
        }
    }

    @CalledByNative
    public void OnNotificationAdded(String id, int type, long timestamp,
            String[] args) {
        for (adrbrowsielRewardsObserver observer : mObservers) {
            observer.OnNotificationAdded(id, type, timestamp, args);
        }
    }

    @CalledByNative
    public void OnNotificationsCount(int count) {
        for (adrbrowsielRewardsObserver observer : mObservers) {
            observer.OnNotificationsCount(count);
        }
    }

    @CalledByNative
    public void OnGetLatestNotification(String id, int type, long timestamp,
            String[] args) {
        for (adrbrowsielRewardsObserver observer : mObservers) {
            observer.OnGetLatestNotification(id, type, timestamp, args);
        }
    }

    @CalledByNative
    public void OnNotificationDeleted(String id) {
        for (adrbrowsielRewardsObserver observer : mObservers) {
            observer.OnNotificationDeleted(id);
        }
    }

    @CalledByNative
    public void OnGetPendingContributionsTotal(double amount) {
        for (adrbrowsielRewardsObserver observer : mObservers) {
            observer.OnGetPendingContributionsTotal(amount);
        }
    }

    @CalledByNative
    public void OnGetAutoContributeProperties() {
        for (adrbrowsielRewardsObserver observer : mObservers) {
            observer.OnGetAutoContributeProperties();
        }
    }

    @CalledByNative
    public void OnGetReconcileStamp(long timestamp) {
        for (adrbrowsielRewardsObserver observer : mObservers) {
            observer.OnGetReconcileStamp(timestamp);
        }
    }

    @CalledByNative
    public void OnRecurringDonationUpdated() {
        for (adrbrowsielRewardsObserver observer : mObservers) {
            observer.OnRecurringDonationUpdated();
        }
    }

    @CalledByNative
    public void OnGrantFinish(int result) {
        grantClaimInProcess = false;
        for(adrbrowsielRewardsObserver observer : mObservers) {
            observer.OnGrantFinish(result);
        }
    }

    @CalledByNative
    public void OnResetTheWholeState(boolean success) {
        for (adrbrowsielRewardsObserver observer : mObservers) {
            observer.OnResetTheWholeState(success);
        }
    }

    @CalledByNative
    public void OnGetExternalWallet(int error_code, String external_wallet) {
        for (adrbrowsielRewardsObserver observer : mObservers) {
            observer.OnGetExternalWallet(error_code, external_wallet);
        }
    }

    @CalledByNative
    public void OnDisconnectWallet(int error_code, String external_wallet) {
        for (adrbrowsielRewardsObserver observer : mObservers) {
            observer.OnDisconnectWallet(error_code, external_wallet);
        }
    }

    @CalledByNative
    public void OnProcessRewardsPageUrl(int error_code, String wallet_type,
            String action, String json_args) {
        for (adrbrowsielRewardsObserver observer : mObservers) {
            observer.OnProcessRewardsPageUrl(error_code, wallet_type,
                    action, json_args);
        }
    }

    @CalledByNative
    public void OnClaimPromotion(int error_code) {
        grantClaimInProcess = false;
        for (adrbrowsielRewardsObserver observer : mObservers) {
            observer.OnClaimPromotion(error_code);
        }
    }

    @CalledByNative
    public void OnOneTimeTip() {
        for (adrbrowsielRewardsObserver observer : mObservers) {
            observer.OnOneTimeTip();
        }
    }

    @NativeMethods
    interface Natives {
        void init(adrbrowsielRewardsNativeWorker caller);
        void destroy(long nativeadrbrowsielRewardsNativeWorker);
        String getWalletBalance(long nativeadrbrowsielRewardsNativeWorker);
        double getWalletRate(long nativeadrbrowsielRewardsNativeWorker);
        void getPublisherInfo(long nativeadrbrowsielRewardsNativeWorker, int tabId, String host);
        String getPublisherURL(long nativeadrbrowsielRewardsNativeWorker, int tabId);
        String getPublisherFavIconURL(long nativeadrbrowsielRewardsNativeWorker, int tabId);
        String getPublisherName(long nativeadrbrowsielRewardsNativeWorker, int tabId);
        String getPublisherId(long nativeadrbrowsielRewardsNativeWorker, int tabId);
        int getPublisherPercent(long nativeadrbrowsielRewardsNativeWorker, int tabId);
        boolean getPublisherExcluded(long nativeadrbrowsielRewardsNativeWorker, int tabId);
        int getPublisherStatus(long nativeadrbrowsielRewardsNativeWorker, int tabId);
        void includeInAutoContribution(
                long nativeadrbrowsielRewardsNativeWorker, int tabId, boolean exclude);
        void removePublisherFromMap(long nativeadrbrowsielRewardsNativeWorker, int tabId);
        void getCurrentBalanceReport(long nativeadrbrowsielRewardsNativeWorker);
        void donate(long nativeadrbrowsielRewardsNativeWorker, String publisher_key, int amount,
                boolean recurring);
        void getAllNotifications(long nativeadrbrowsielRewardsNativeWorker);
        void deleteNotification(long nativeadrbrowsielRewardsNativeWorker, String notification_id);
        void getGrant(long nativeadrbrowsielRewardsNativeWorker, String promotionId);
        String[] getCurrentGrant(long nativeadrbrowsielRewardsNativeWorker, int position);
        void getPendingContributionsTotal(long nativeadrbrowsielRewardsNativeWorker);
        void getRecurringDonations(long nativeadrbrowsielRewardsNativeWorker);
        boolean isCurrentPublisherInRecurrentDonations(
                long nativeadrbrowsielRewardsNativeWorker, String publisher);
        void getAutoContributeProperties(long nativeadrbrowsielRewardsNativeWorker);
        boolean isAutoContributeEnabled(long nativeadrbrowsielRewardsNativeWorker);
        void getReconcileStamp(long nativeadrbrowsielRewardsNativeWorker);
        double getPublisherRecurrentDonationAmount(
                long nativeadrbrowsielRewardsNativeWorker, String publisher);
        void removeRecurring(long nativeadrbrowsielRewardsNativeWorker, String publisher);
        void resetTheWholeState(long nativeadrbrowsielRewardsNativeWorker);
        void fetchGrants(long nativeadrbrowsielRewardsNativeWorker);
        int getAdsPerHour(long nativeadrbrowsielRewardsNativeWorker);
        void setAdsPerHour(long nativeadrbrowsielRewardsNativeWorker, int value);
        boolean isAnonWallet(long nativeadrbrowsielRewardsNativeWorker);
        boolean isRewardsEnabled(long nativeadrbrowsielRewardsNativeWorker);
        void getExternalWallet(long nativeadrbrowsielRewardsNativeWorker);
        void disconnectWallet(long nativeadrbrowsielRewardsNativeWorker);
        void processRewardsPageUrl(long nativeadrbrowsielRewardsNativeWorker, String path, String query);
        void recoverWallet(long nativeadrbrowsielRewardsNativeWorker, String passPhrase);
        void refreshPublisher(long nativeadrbrowsielRewardsNativeWorker, String publisherKey);
        void getRewardsParameters(long nativeadrbrowsielRewardsNativeWorker);
        void setAutoContributeEnabled(
                long nativeadrbrowsielRewardsNativeWorker, boolean isSetAutoContributeEnabled);
        void setAutoContributionAmount(long nativeadrbrowsielRewardsNativeWorker, double amount);
        void startProcess(long nativeadrbrowsielRewardsNativeWorker);
    }
}
