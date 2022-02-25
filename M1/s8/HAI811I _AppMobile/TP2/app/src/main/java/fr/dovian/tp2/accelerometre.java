package fr.dovian.tp2;

import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.widget.TextView;

public class accelerometre extends AppCompatActivity implements SensorEventListener {

    ConstraintLayout background;
    long lastUpdate = 0;

    private float lastX = (float) 0.26;
    private float lastY = (float) 0.05;
    private float lastZ = (float) 9.82;

    public void onSensorChanged(SensorEvent event) {
    // Récupérer les valeurs du capteur
        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            long curTime = System.currentTimeMillis();
            if ((curTime - lastUpdate) > 50) {
                lastUpdate = curTime;

                float deltaX = Math.abs(lastX - event.values[0]);
                float deltaY = Math.abs(lastY - event.values[1]);
                float deltaZ = Math.abs(lastZ - event.values[2]);

                if (deltaX >= 10 || deltaY >= 10 || deltaZ >= 10) {
                    background.setBackgroundColor(getResources().getColor(android.R.color.holo_red_light));
                } else if (deltaX >= 1 || deltaY >= 1 || deltaZ >= 1) {
                    background.setBackgroundColor(getResources().getColor(android.R.color.black));
                } else {
                    background.setBackgroundColor(getResources().getColor(android.R.color.holo_green_light));
                }

                lastX = event.values[0];
                lastY = event.values[1];
                lastZ = event.values[2];
            }
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.accelerometre);

        background = findViewById(R.id.background);

        SensorManager sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        Sensor sensor = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        sensorManager.registerListener(this, sensor, SensorManager.SENSOR_DELAY_NORMAL);
    }
}
