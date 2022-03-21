package fr.dovian.tp2;

import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

public class direction extends AppCompatActivity implements SensorEventListener {

    ImageView up;
    ImageView down;
    ImageView left;
    ImageView right;

    private float lastX = 0.26f;
    private float lastY = 0.05f;
    private float lastZ = 9.82f;

    public void onSensorChanged(SensorEvent event) {
        // reinitialiser les scales
        left.setScaleX(1f);
        left.setScaleY(1f);
        right.setScaleX(1f);
        right.setScaleY(1f);
        up.setScaleX(1f);
        up.setScaleY(1f);
        down.setScaleX(1f);
        down.setScaleY(1f);

        // Récupérer les valeurs du capteur
        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            float directionX = event.values[0] - lastX;
            float directionY =  event.values[1] - lastY;
            float deltaX = Math.abs(lastX - event.values[0]);
            float deltaY = Math.abs(lastY - event.values[1]);

            // droite
            if (deltaX > 3 && directionX < 0) {
                right.setScaleX(2.5f);
                right.setScaleY(2.5f);
            }
            // gauche
            else if (deltaX > 3 && directionX > 0) {
                left.setScaleX(2.5f);
                left.setScaleY(2.5f);
            }
            // haut
            else if (deltaY > 3 && directionY < 0) {
                up.setScaleX(2.5f);
                up.setScaleY(2.5f);
            }
            // bas
            else if (deltaY > 3 && directionY > 0) {
                down.setScaleX(2.5f);
                down.setScaleY(2.5f);
            }
            lastX = event.values[0];
            lastY = event.values[1];
            lastZ = event.values[2];
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.direction);

        up = findViewById(R.id.haut);
        down = findViewById(R.id.bas);
        left = findViewById(R.id.left);
        right = findViewById(R.id.right);

        SensorManager sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        Sensor sensor = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        if (sensor == null) {
            Toast.makeText(this, "No accelerometer sensor found in device.", Toast.LENGTH_SHORT).show();
            finish();
        } else sensorManager.registerListener(this, sensor, SensorManager.SENSOR_DELAY_NORMAL);
    }
}
