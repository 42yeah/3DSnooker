package aiofwa.org.a3dsnooker;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.Build;
import android.os.IBinder;
import android.support.annotation.Nullable;
import android.support.v4.app.NotificationCompat;


public class NotificationBlabber extends Service {
    public NotificationBlabber() {
        channelID = "Snooker3DChannel";
        channelName = "Snooker3D";
        notificationID = 1;
        channelDescription = "Playing Snooker on an ACTUAL table! Imagine that!";
        binder = new BlabberBinder(this);
    }

    @Override
    public void onCreate() {
        super.onCreate();

        NotificationManager manager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);

        // Oreo requires creating the channel first.
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            int importance = NotificationManager.IMPORTANCE_LOW;
            NotificationChannel channel = new NotificationChannel(channelID, channelName, importance);
            channel.setDescription(channelDescription);
            manager.createNotificationChannel(channel);
        }
    }

    public void blab(String content) {
        NotificationManager manager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);

        NotificationCompat.Builder builder = new NotificationCompat.Builder(this, channelID)
                .setSmallIcon(R.drawable.eight)
                .setContentTitle("Snooker3D")
                .setContentText(content)
                .setPriority(NotificationCompat.PRIORITY_LOW)
                .setAutoCancel(true);

        manager.notify(notificationID, builder.build());
    }

    /**
     * onBind is not to be implemented.
     * @param intent the intent
     * @return null
     */
    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return binder;
    }

    private String channelID;
    private String channelName;
    private String channelDescription;
    private int notificationID;
    private BlabberBinder binder;

    class BlabberBinder extends Binder {
        public BlabberBinder() {}
        public BlabberBinder(NotificationBlabber service) {
            this.service = service;
        }

        public NotificationBlabber getService() {
            return service;
        }

        private NotificationBlabber service;
    }
}
