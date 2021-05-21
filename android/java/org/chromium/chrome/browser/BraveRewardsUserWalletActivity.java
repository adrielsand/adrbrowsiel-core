/** Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v. 2.0. If a copy of the MPL was not distributed with this file,
  * You can obtain one at http://mozilla.org/MPL/2.0/.
  */

package org.chromium.chrome.browser;

import android.content.Intent;
import android.os.Bundle;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;

import org.chromium.chrome.R;
import org.chromium.chrome.browser.adrbrowsielRewardsBalance;
import org.chromium.chrome.browser.adrbrowsielRewardsExternalWallet;
import org.chromium.chrome.browser.adrbrowsielRewardsNativeWorker;
import org.chromium.chrome.browser.app.adrbrowsielActivity;

public class adrbrowsielRewardsUserWalletActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.user_wallet_activity);

        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        ActionBar ab = getSupportActionBar();
        ab.setDisplayHomeAsUpEnabled(true);
        ab.setDisplayShowTitleEnabled(false);

        SetUIControls();
    }

    private void SetUIControls() {
        Intent intent = getIntent();
        final int status = intent.getIntExtra(
                adrbrowsielRewardsExternalWallet.STATUS, -1);
        TextView txtUserId = (TextView)findViewById(R.id.user_id);
        TextView txtUserStatus = (TextView)findViewById(R.id.user_status);
        Button btn1 = (Button)findViewById(R.id.user_wallet_btn1);
        Button btn2 = null;
        Button btnGotoUphold = (Button)findViewById(
                R.id.user_wallet_go_to_uphold);

        if (status < adrbrowsielRewardsExternalWallet.NOT_CONNECTED ||
                status > adrbrowsielRewardsExternalWallet.PENDING) {
            finish();
        }
        else if (status == adrbrowsielRewardsExternalWallet.VERIFIED){
          //set 2nd button visible
          findViewById(R.id.user_wallet_btn2_separator).setVisibility(
                  View.VISIBLE);
          btn2 = (Button)findViewById(R.id.user_wallet_btn2);
          btn2.setVisibility(View.VISIBLE);

          //Buttons:
          //Add funds
          //Withdraw
          //Go to uphold
          //Disconnect
          btn1.setText(getResources().getString(
                  R.string.adrbrowsiel_rewards_local_panel_add_funds));
          btn2.setText(getResources().getString(
                  R.string.user_wallet_withdraw_funds));
          txtUserStatus.setText(
                  adrbrowsielRewardsExternalWallet.WalletStatusToString(status));

          SetBtnOpenUrlClickHandler(btn1,
                  intent.getStringExtra(adrbrowsielRewardsExternalWallet.ADD_URL));
          SetBtnOpenUrlClickHandler(btn2,
                  intent.getStringExtra(adrbrowsielRewardsExternalWallet.WITHDRAW_URL));
        }
        else {
          //CONNECTED or PENDING
          //Buttons:
          //Complete verification
          //Go to uphold
          //Disconnect
          btn1.setText(getResources().getString(
                  R.string.user_wallet_complete_verification));
          SetBtnOpenUrlClickHandler(btn1,
                  intent.getStringExtra(adrbrowsielRewardsExternalWallet.VERIFY_URL));
        }

        SetBtnOpenUrlClickHandler(btnGotoUphold,
                  intent.getStringExtra(adrbrowsielRewardsExternalWallet.ACCOUNT_URL));

        String userId = intent.getStringExtra(adrbrowsielRewardsExternalWallet.USER_NAME);
        txtUserId.setText(userId);
        SetDisconnectBtnClickHandler();
    }

    private void SetBtnOpenUrlClickHandler(Button btn, String url) {
      btn.setOnClickListener( (View v) -> {
              Intent intent = new Intent();
              intent.putExtra(adrbrowsielActivity.OPEN_URL, url);
              setResult(RESULT_OK, intent);
              finish();
      });
    }

    private void SetDisconnectBtnClickHandler() {
        Button btnDisconnect = (Button)findViewById(
                R.id.user_wallet_disconnect);
        btnDisconnect.setOnClickListener((View v) -> {
            adrbrowsielRewardsNativeWorker.getInstance().DisconnectWallet();
            Intent intent = new Intent();
            setResult(RESULT_OK, intent);
            finish();
        });
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if(item.getItemId()==android.R.id.home) {
            finish();
        }
        return super.onOptionsItemSelected(item);
    }
}
