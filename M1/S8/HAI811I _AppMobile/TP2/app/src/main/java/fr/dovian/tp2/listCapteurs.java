package fr.dovian.tp2;

import androidx.appcompat.app.AppCompatActivity;

import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import java.util.ArrayList;
import java.util.List;

public class listCapteurs extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.list_capteurs);
        SensorManager sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        List<Sensor> sensorsList = sensorManager.getSensorList(Sensor.TYPE_ALL);

        List<StringBuffer> strSensorsList = new ArrayList<>();
        StringBuffer sensorDesc;
        for (Sensor sensor : sensorsList) {
            sensorDesc = new StringBuffer();
            sensorDesc.append("\n");
            sensorDesc.append("New sensor detected : \r\n");
            sensorDesc.append("\tName: " + sensor.getName() + "\r\n");
            sensorDesc.append("\tType: " + sensor.getType() + "\r\n");
            sensorDesc.append("Version: " + sensor.getVersion() + "\r\n");
            sensorDesc.append("Resolution (in the sensor unit): " +
                    sensor.getResolution() + "\r\n");
            sensorDesc.append("Power in mA used by this sensor while in use" +
                    sensor.getPower() +"\r\n");
            sensorDesc.append("Vendor: " + sensor.getVendor() + "\r\n");
            sensorDesc.append("Maximum range of the sensor in the sensor's unit." +
                    sensor.getMaximumRange() + "\r\n");
            sensorDesc.append("Minimum delay allowed between two events in microsecond" + "or zero if this sensor only returns a " +
                    "value when the data its measuring changes " +
            sensor.getMinDelay() + "\r\n");
            sensorDesc.append("\n");
            strSensorsList.add(sensorDesc);
        }

        ArrayAdapter<StringBuffer> arrayAdapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, strSensorsList);
        ListView sensorsListView = findViewById(R.id.capteurs);
        sensorsListView.setAdapter(arrayAdapter);
    }
}