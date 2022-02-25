package fr.dovian.tp2;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;

import java.util.ArrayList;
import java.util.List;

public class menu extends AppCompatActivity {

    public void addListenerOnButton(){
        Button listCapteurs = findViewById(R.id.listCapteurs);
        Button presenceCapteurs = findViewById(R.id.presenceCapteurs);
        Button accelerometre = findViewById(R.id.accelerometre);
        Button direction = findViewById(R.id.direction);
        Button lampeTorche = findViewById(R.id.lampeTorche);
        listCapteurs.setOnClickListener(view -> {
            Intent intent = new Intent (this, listCapteurs.class);
            startActivity(intent);
        });
        presenceCapteurs.setOnClickListener(view -> {
            Intent intent = new Intent (this, presenceCapteurs.class);
            startActivity(intent);
        });
        accelerometre.setOnClickListener(view -> {
            Intent intent = new Intent (this, accelerometre.class);
            startActivity(intent);
        });
        direction.setOnClickListener(view -> {
            Intent intent = new Intent (this, direction.class);
            startActivity(intent);
        });
        lampeTorche.setOnClickListener(view -> {
            Intent intent = new Intent (this, lampeTorche.class);
            startActivity(intent);
        });
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.menu);
        addListenerOnButton();
    }
}