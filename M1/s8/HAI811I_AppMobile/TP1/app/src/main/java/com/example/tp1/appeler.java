package com.example.tp1;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.example.tp1.R;

public class appeler extends AppCompatActivity {

    public void addListenerOnButton(){
        Button buttonCall = findViewById(R.id.call);
        buttonCall.setOnClickListener(view -> {
            EditText phoneNumberEdit = findViewById(R.id.phoneToCall);
            String phoneNumber = phoneNumberEdit.getText().toString();
            Intent phoneIntent = new Intent(Intent.ACTION_DIAL, Uri.parse("tel:"+phoneNumber));
            startActivity(phoneIntent);
        });
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.appeler);
        addListenerOnButton();
    }
}