package com.example.tp1;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;

public class horaires extends AppCompatActivity {

    public String[] getHoraires(String aller, String retour) {
        String[] horaires = new String[15];
        for (int i = 0, h = 6;  h < 21; i++, h++) {
            String min = "37";
            String horaire = "\n" + Integer.toString(h) + "h" + min + " " + aller + "\n" +
                    Integer.toString(h+2) + "h" + min + " " + retour + "\n";
            horaires[i] = horaire;
        }
        return horaires;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.horaires);
        Intent intent = getIntent();
        String aller = intent.getStringExtra("aller");
        String retour = intent.getStringExtra("retour");
        String[] horaires = getHoraires(aller, retour);
        ArrayAdapter<String> arrayAdapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, horaires);
        ListView listeHoraires = findViewById(R.id.horaires);
        listeHoraires.setAdapter(arrayAdapter);
    }
}