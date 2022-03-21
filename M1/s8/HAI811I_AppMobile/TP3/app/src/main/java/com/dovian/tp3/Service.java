package com.dovian.tp3;

import android.app.IntentService;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.widget.Toast;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.URL;
import java.util.Timer;
import java.util.TimerTask;


public class Service extends IntentService {

    public Service() {
        super("Service");
    }

    @Override
    protected void onHandleIntent(Intent intent) {
        Context context = getApplicationContext();
        int duration = Toast.LENGTH_SHORT;
        HttpURLConnection urlConnection = null;
        URL url = null;
        try {
            url = new URL("https://api.chucknorris.io/jokes/random");
        } catch (MalformedURLException e) {
            e.printStackTrace();
        }
        try {
            urlConnection = (HttpURLConnection) url.openConnection();
        } catch (IOException e) {
            e.printStackTrace();
        }
        try {
            urlConnection.setRequestMethod("GET");
        } catch (ProtocolException e) {
            e.printStackTrace();
        }
        urlConnection.setReadTimeout(10000);
        urlConnection.setConnectTimeout(15000);
        urlConnection.setDoOutput(true);
        try {
            urlConnection.connect();
        } catch (IOException e) {
            e.printStackTrace();
        }

        BufferedReader br = null;
        try {
            br = new BufferedReader(new InputStreamReader(url.openStream()));
        } catch (IOException e) {
            e.printStackTrace();
        }
        StringBuilder sb = new StringBuilder();

        String line = "";
        while (true) {
            try {
                if ((line = br.readLine()) == null) break;
            } catch (IOException e) {
                e.printStackTrace();
            }
            sb.append(line + "\n");
        }
        try {
            br.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        String jsonString = sb.toString();

        String filename = "file.json";
        String string;

        FileOutputStream fos = null;

        string = jsonString;
        try {
            fos = getApplicationContext().openFileOutput(filename, Context.MODE_PRIVATE);
            fos.write(string.getBytes());
            fos.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        String text = "";
        for (int i = 0; i < getApplicationContext().fileList().length; i++){
            if(i == getApplicationContext().fileList().length - 1) {
                text += getApplicationContext().fileList()[i];
            } else text += getApplicationContext().fileList()[i] + ", ";
        }
        text += " téléchargé(s) !";
        Toast toast = Toast.makeText(context, text, duration);
        toast.show();

        stopSelf();
    }
}