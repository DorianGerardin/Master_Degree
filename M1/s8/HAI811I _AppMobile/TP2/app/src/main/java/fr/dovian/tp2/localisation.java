package fr.dovian.tp2;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.location.Address;
import android.location.Geocoder;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.util.Log;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.List;
import java.util.Locale;

public class localisation extends AppCompatActivity {

    TextView longitudeTv;
    TextView latitudeTv;
    TextView adresseTv;
    LocationManager locationManager;

    private String getCompleteAddressString(double LATITUDE, double LONGITUDE) {
        String strAdd = "";
        Geocoder geocoder = new Geocoder(this, Locale.getDefault());
        try {
            List<Address> addresses = geocoder.getFromLocation(LATITUDE, LONGITUDE, 1);
            if (addresses != null) {
                Address returnedAddress = addresses.get(0);
                StringBuilder strReturnedAddress = new StringBuilder("");
                for (int i = 0; i <= returnedAddress.getMaxAddressLineIndex(); i++) {
                    strReturnedAddress.append(returnedAddress.getAddressLine(i)).append("\n");
                }
                strAdd = strReturnedAddress.toString();
            } else {
                strAdd = "Pas d'adresse trouvée";
            }
        } catch (Exception e) {
            e.printStackTrace();
            strAdd = "Impossible de récupérer une adresse";
        }
        return strAdd;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.localisation);
        longitudeTv = findViewById(R.id.longitude);
        latitudeTv = findViewById(R.id.latitude);
        adresseTv = findViewById(R.id.adresse);
        locationManager = (LocationManager)getSystemService(Context.LOCATION_SERVICE);

        if(ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED &&
                ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED)
        {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.ACCESS_COARSE_LOCATION, Manifest.permission.ACCESS_FINE_LOCATION}, 1);
            locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 10, 5, new LocationListener() {
                @Override
                public void onLocationChanged(Location location) {
                    double latitude = location.getLatitude();
                    double longitude = location.getLongitude();
                    longitudeTv.setText("Longitude : " + longitude);
                    latitudeTv.setText("Latitude : " + latitude);
                    adresseTv.setText("Adresse : " + getCompleteAddressString(latitude, longitude));
                }
            });
        } else {
            locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 10, 5, new LocationListener() {
                @Override
                public void onLocationChanged(Location location) {
                    double latitude = location.getLatitude();
                    double longitude = location.getLongitude();
                    longitudeTv.setText("Longitude : " + longitude);
                    latitudeTv.setText("Latitude : " + latitude);
                    adresseTv.setText("Adresse : " + getCompleteAddressString(latitude, longitude));
                }
            });
        }



    }
}
