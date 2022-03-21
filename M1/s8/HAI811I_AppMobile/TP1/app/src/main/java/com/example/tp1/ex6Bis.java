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
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.example.tp1.R;

public class ex6Bis extends AppCompatActivity {

    public void addListenerOnButton(){
        Button buttonCall = findViewById(R.id.call);
        Button buttonRetour = findViewById(R.id.Retour);
        Button buttonTrains = findViewById(R.id.trains);
        buttonCall.setOnClickListener(view -> {
            Intent intent = new Intent (this, appeler.class);
            startActivity(intent);
        });

        buttonRetour.setOnClickListener(view -> {
            this.finish();
        });

        buttonTrains.setOnClickListener(view -> {
            Intent intent = new Intent (this, trains.class);
            startActivity(intent);
        });
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.ex6_bis);
        addListenerOnButton();
        final Intent intent = getIntent();
        String name = intent.getStringExtra("name");
        String surname = intent.getStringExtra("surname");
        String age = intent.getStringExtra("age");
        String skill = intent.getStringExtra("skill");
        String phone = intent.getStringExtra("phone");
        final TextView tv = findViewById( R.id.message );
        String message = "Bonjour " + surname + " " + name + ", aujourd'hui vous avez " + age + " ans. \nVos(votre) compétence(s) sont(est) " + skill
                + ". \nVotre numéro de téléphone est : " + phone;
        tv.setText( message );
    }
}