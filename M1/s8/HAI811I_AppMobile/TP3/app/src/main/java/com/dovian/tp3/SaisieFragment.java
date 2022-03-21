package com.dovian.tp3;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Switch;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

public class SaisieFragment extends Fragment {

    EditText firstNameEdit, surnameEdit, ageEdit, mailEdit, phoneEdit;
    CheckBox sport, music, gaming;
    Switch sync;
    Button submit, download;
    TextView affichageJson;

    public String mReadJsonData(String filename) {
        try {
            Context context = getContext();
            File f = new File("/data/data/" + context.getPackageName() + "/files/" + filename);
            FileInputStream is = new FileInputStream(f);
            int size = is.available();
            byte[] buffer = new byte[size];
            is.read(buffer);
            is.close();
            return new String(buffer);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return "";
    }

    public void addListenerOnButton(){
        submit = getView().findViewById(R.id.submit);
        download = getView().findViewById(R.id.download);

        submit.setOnClickListener(view -> {
            Bundle bundle = new Bundle();
            bundle.putString("surname", String.valueOf(surnameEdit.getText()));
            bundle.putString("firstName", String.valueOf(firstNameEdit.getText()));
            bundle.putString("age", String.valueOf(ageEdit.getText()));
            bundle.putString("phoneNumber", String.valueOf(phoneEdit.getText()));
            bundle.putString("mailAddress", String.valueOf(mailEdit.getText()));

            bundle.putBoolean("sport", sport.isChecked());
            bundle.putBoolean("gaming", gaming.isChecked());
            bundle.putBoolean("music", music.isChecked());
            bundle.putBoolean("sync", sync.isChecked());

            FragmentManager fragmentManager = getParentFragmentManager();
            FragmentTransaction fragmentTransaction = fragmentManager.beginTransaction();
            fragmentTransaction.setReorderingAllowed(true);
            fragmentTransaction.replace(R.id.fragment, AffichageFragment.class, bundle);
            fragmentTransaction.addToBackStack("Saisie");
            fragmentTransaction.commit();
        });

        download.setOnClickListener(view -> {
            Intent downloadIntent = new Intent(getContext(), Service.class);
            getContext().startService(downloadIntent);
            String jsonDataString = mReadJsonData("file.json");
            try {
                JSONObject jsonData = new JSONObject(jsonDataString);
                String fact = jsonData.getString("value");
                affichageJson.setText("Fait de chuck Norris : \n" + fact);
            } catch (JSONException e) {
                e.printStackTrace();
            }
        });
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.saisie_fragment, container, false);
    }

    @Override
    public void onViewCreated(@NonNull View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        surnameEdit = getView().findViewById(R.id.nomEdit);
        firstNameEdit = getView().findViewById(R.id.prenomEdit);
        ageEdit = getView().findViewById(R.id.AgeEdit);
        mailEdit = getView().findViewById(R.id.MailEdit);
        phoneEdit = getView().findViewById(R.id.phoneEdit);
        sport = getView().findViewById(R.id.SportCB);
        music = getView().findViewById(R.id.MusicCB);
        gaming = getView().findViewById(R.id.gamingCB);
        sync = getView().findViewById(R.id.switchSync);
        affichageJson = getView().findViewById(R.id.affichageDownload);
        addListenerOnButton();
    }

}
