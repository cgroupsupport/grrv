package com.cgroup.androidvision;

import android.content.Context;
import android.content.Intent;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Toast;

/** ConnectActivity is the first interface that user will see.
 */
public class ConnectActivity extends AppCompatActivity {

    private WifiManager mWifiManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_connect);
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

        mWifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);
    }

    /** Quit Button, by clicking on this button, user can quit the application.
     * @param view
     */
    public void quit(View view) {
        moveTaskToBack(true);
        android.os.Process.killProcess(android.os.Process.myPid());
    }

    /** Connect Button, by clicking on this button, user can enter the Video Activity
     * @param view
     */
    public void connect(View view){
        if (mWifiManager.getConnectionInfo().getSSID().equals("\"CgroupRasPi\"")) {
            Intent intent = new Intent(this, VideoActivity.class);
            startActivity(intent);
            finish();
        } else {
            Toast.makeText(getApplicationContext(), "Choose the CgroupRasPi network", Toast.LENGTH_SHORT).show();
            startActivityForResult(new Intent(WifiManager.ACTION_PICK_WIFI_NETWORK), 1);
        }
    }

    @Override
    /** User must be on the correct network in order to proceed to the Video Activity.
     */
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (mWifiManager.getConnectionInfo().getSSID().equals("\"CgroupRasPi\"")) { // si le bon wifi
            Intent intent = new Intent(this, VideoActivity.class);
            startActivity(intent);
            finish();
        } else {
            Toast.makeText(getApplicationContext(), "Select the correct network", Toast.LENGTH_SHORT).show();
        }
    }

    /** Back Button, by pressing the back button, user can close the application
     */
    @Override
    public void onBackPressed() {
        super.onBackPressed();
        moveTaskToBack(true);
        android.os.Process.killProcess(android.os.Process.myPid());
    }

    /** onDestroy, when this activity is destroyed, it will close Connect Activity.*/
    @Override
    public void onDestroy() {
        super.onDestroy();
        finish();
    }
}
