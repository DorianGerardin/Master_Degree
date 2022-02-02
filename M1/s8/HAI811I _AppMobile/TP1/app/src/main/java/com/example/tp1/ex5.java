package com.example.tp1;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.content.DialogInterface;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.example.tp1.R;

public class ex5 extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Button button = (Button) findViewById(R.id.submit);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                AlertDialog.Builder builder = new AlertDialog.Builder(ex5.this);
                builder.setTitle("Confirmation").setMessage("Voulez vous accéder aux horaires de train ?");
                builder.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        Toast.makeText(ex5.this,"Tu regardes les horaires de train",
                                Toast.LENGTH_SHORT).show();
                        Activity activity = (Activity) ex5.this;
                        activity.finish();
                    }
                });
                builder.setNegativeButton("no", null);
            }
        });
        setContentView(R.layout.ex5);
    }
}