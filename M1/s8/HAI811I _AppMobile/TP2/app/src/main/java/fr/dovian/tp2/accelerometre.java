package fr.dovian.tp2;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

public class accelerometre extends AppCompatActivity implements SensorEventListener {

    TextView tvX;
    TextView tvY;
    TextView tvZ;

    public void onSensorChanged(SensorEvent event) {
    // Récupérer les valeurs du capteur
        float x, y, z;
        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            x = Math.abs(event.values[0]);
            y = Math.abs(event.values[1]);
            z = Math.abs(event.values[2]);
            if (x > 5 || y > 5 || z > 5) {
                tvX.setText("x : " + x);
                tvY.setText("y : " + y);
                tvZ.setText("z : " + z);
                Toast.makeText(this, "shake function activated", Toast.LENGTH_SHORT).show();
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

        tvX = findViewById(R.id.x) ;
        tvY = findViewById(R.id.y) ;
        tvZ = findViewById(R.id.z) ;

        tvX.setText("x : ");
        tvY.setText("y : ");
        tvZ.setText("z : ");

        SensorManager sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        Sensor sensor = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        sensorManager.registerListener(this, sensor, SensorManager.SENSOR_DELAY_NORMAL);
    }
}
