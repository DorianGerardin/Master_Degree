package com.example.tp1;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.example.tp1.R;


public class trains extends AppCompatActivity implements AdapterView.OnItemSelectedListener {

    private String aller = "";
    private String retour = "";

    public void onItemSelected(AdapterView<?> parent, View view,
                               int pos, long id) {
        Spinner spinner = (Spinner) parent;
        if(spinner.getId() == R.id.aller)
        {
            aller = parent.getItemAtPosition(pos).toString();

        }
        else if(spinner.getId() == R.id.retour)
        {
            retour = parent.getItemAtPosition(pos).toString();
        }
    }

    public void onNothingSelected(AdapterView<?> parent) {
        // Another interface callback
    }

    public void addListenerOnButton(){
        Button buttonValider = findViewById(R.id.seeTrains);
        buttonValider.setOnClickListener(view -> {
            Intent intent = new Intent(this, horaires.class);
            intent.putExtra("aller", this.aller);
            intent.putExtra("retour", this.retour);
            startActivity(intent);
        });
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.trains);
        addListenerOnButton();
        Spinner aller = findViewById(R.id.aller);
        aller.setOnItemSelectedListener(this);
        Spinner retour = findViewById(R.id.retour);
        retour.setOnItemSelectedListener(this);
        // Create an ArrayAdapter using the string array and a default spinner layout
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this,
                R.array.cities, android.R.layout.simple_spinner_item);
        // Specify the layout to use when the list of choices appears
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        // Apply the adapter to the spinner
        aller.setAdapter(adapter);
        retour.setAdapter(adapter);
    }
}

