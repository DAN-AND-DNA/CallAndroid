package cn.afternoontea.ue4;


import android.app.Activity;
import android.provider.Settings;

public class DeviceInfo {
    public static String GetAndroidID(Activity activity) {
        if (activity != null) {
            return Settings.Secure.getString(activity.getContentResolver(), Settings.Secure.ANDROID_ID);
        }
        return "";
    }
}
