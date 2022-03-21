package fr.dovian.tp2;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;

public class presenceCapteurs extends AppCompatActivity {

    @SuppressLint("ResourceAsColor")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        LinearLayout container = new LinearLayout(this);
        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        params.setMargins(20,20,20,20);
        container.setOrientation(LinearLayout.VERTICAL);
        container.setGravity(Gravity.CENTER_HORIZONTAL);

        SensorManager sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        Sensor defaultProximitySensor = sensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY);
        Sensor defaultAccelerometerSensor = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        Sensor defaultHeartBeatSensor = sensorManager.getDefaultSensor(Sensor.TYPE_HEART_BEAT);
        Sensor defaultGyroscopeSensor = sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);

        TextView tvProximity = new TextView(this);
        tvProximity.setPadding(20,20,20, 20);
        tvProximity.setLayoutParams(params);
        if (defaultProximitySensor != null) {
            tvProximity.setText("Proximité \n\t Présence du capteur : Oui");
            tvProximity.setBackgroundColor(getResources().getColor(android.R.color.holo_green_light));
        } else {
            tvProximity.setText("Proximité \n\t Présence du capteur : Non");
            tvProximity.setBackgroundColor(getResources().getColor(android.R.color.holo_red_light));
        }

        TextView tvAcceleromter = new TextView(this);
        tvAcceleromter.setPadding(20,20,20, 20);
        tvAcceleromter.setLayoutParams(params);
        if (defaultAccelerometerSensor != null) {
            tvAcceleromter.setText("Accéléromètre \n\t Présence du capteur : Oui");
            tvAcceleromter.setBackgroundColor(getResources().getColor(android.R.color.holo_green_light));
        } else {
            tvAcceleromter.setText("Accéléromètre \n\t Présence du capteur : Non");
            tvAcceleromter.setBackgroundColor(getResources().getColor(android.R.color.holo_red_light));
        }

        TextView tvHeartBeat = new TextView(this);
        tvHeartBeat.setPadding(20,20,20, 20);
        tvHeartBeat.setLayoutParams(params);
        if (defaultHeartBeatSensor != null) {
            tvHeartBeat.setText("Battement du coeur \n\t Présence du capteur : Oui");
            tvHeartBeat.setBackgroundColor(getResources().getColor(android.R.color.holo_green_light));
        } else {
            tvHeartBeat.setText("Battement du coeur \n\t Présence du capteur : Non");
            tvHeartBeat.setBackgroundColor(getResources().getColor(android.R.color.holo_red_light));
        }

        TextView tvGyroscope = new TextView(this);
        tvGyroscope.setPadding(20,20,20, 20);
        tvGyroscope.setLayoutParams(params);
        if (defaultGyroscopeSensor != null) {
            tvGyroscope.setText("Gyroscope \n\t Présence du capteur : Oui");
            tvGyroscope.setBackgroundColor(getResources().getColor(android.R.color.holo_green_light));
        } else {
            tvGyroscope.setText("Gyroscope \n\t Présence du capteur : Oui");
            tvGyroscope.setBackgroundColor(getResources().getColor(android.R.color.holo_red_light));
        }

        container.addView(tvProximity);
        container.addView(tvAcceleromter);
        container.addView(tvHeartBeat);
        container.addView(tvGyroscope);
        setContentView(container);
    }
}
