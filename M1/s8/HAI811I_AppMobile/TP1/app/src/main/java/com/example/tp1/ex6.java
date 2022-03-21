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
import android.widget.Toast;

import com.example.tp1.R;

public class ex6 extends AppCompatActivity {

    private static final int CODE_MON_ACTIVITE = 1;

    public void addListenerOnButton(){
        Button button = findViewById(R.id.submit);
        button.setOnClickListener(view -> {
            AlertDialog.Builder builder = new AlertDialog.Builder(ex6.this);
            builder.setTitle("Confirmation").setMessage("Voulez vous vous connecter ?");
            builder.setPositiveButton("Oui", (dialog, id) -> {
                Intent intent = new Intent (this, ex6Bis.class);

                EditText nameEdit = findViewById(R.id.nomEdit);
                EditText surnameEdit = findViewById(R.id.prenomEdit);
                EditText ageEdit = findViewById(R.id.AgeEdit);
                EditText skillEdit = findViewById(R.id.SkillEdit);
                EditText phoneEdit = findViewById(R.id.phoneEdit);

                intent.putExtra("name", nameEdit.getText().toString());
                intent.putExtra("surname", surnameEdit.getText().toString());
                intent.putExtra("age", ageEdit.getText().toString());
                intent.putExtra("skill", skillEdit.getText().toString());
                intent.putExtra("phone", phoneEdit.getText().toString());
                startActivity(intent);
            });
            builder.setNegativeButton("Non", (dialog, id) -> {
                Toast.makeText(ex6.this, "Tu as refusé",
                        Toast.LENGTH_SHORT).show();
            });
            AlertDialog dialog = builder.create();
            dialog.show();
        });
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.ex5);
        addListenerOnButton();
    }
}