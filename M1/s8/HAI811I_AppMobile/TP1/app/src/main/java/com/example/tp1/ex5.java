package com.example.tp1;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.content.DialogInterface;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.example.tp1.R;

public class ex5 extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.ex5);
        Button button = findViewById(R.id.submit);
        button.setOnClickListener(view -> {
            AlertDialog.Builder builder = new AlertDialog.Builder(ex5.this);
            builder.setTitle("Confirmation").setMessage("Voulez vous accéder aux horaires de train ?");
            builder.setPositiveButton("Yes", (dialog, id) -> {
                Toast.makeText(ex5.this,"Tu regardes les horaires de train",
                        Toast.LENGTH_SHORT).show();
            });
            builder.setNegativeButton("no", (dialog, id) -> {
                Toast.makeText(ex5.this, "Tu as refusé",
                        Toast.LENGTH_SHORT).show();
            });
            AlertDialog dialog = builder.create();
            dialog.show();
        });
    }
}