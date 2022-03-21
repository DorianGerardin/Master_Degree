package com.dovian.tp3;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.FileOutputStream;
import java.io.IOException;

public class AffichageFragment extends Fragment {

    TextView tv;
    Button cancel, validate;

    public void addListenerOnButton(Bundle bundle){
        cancel = getView().findViewById(R.id.Cancel);
        validate = getView().findViewById(R.id.Valider_Affichage);

        cancel.setOnClickListener(view -> {
            if(!getActivity().getSupportFragmentManager().popBackStackImmediate()){
                getActivity().onBackPressed();
            }
        });

        validate.setOnClickListener(view -> {
            String filename = "userData.json";
            String string;
            FileOutputStream fos = null;
            JSONObject data = new JSONObject();
            try {
                data.put("surname", bundle.getString("surname"));
                data.put("firstName", bundle.getString("firstName"));
                data.put("age", bundle.getString("age"));
                data.put("phoneNumber", bundle.getString("phoneNumber"));
                data.put("mailAddress", bundle.getString("mailAddress"));

                JSONObject hobbies = new JSONObject();
                hobbies.put("sport", bundle.getBoolean("sport"));
                hobbies.put("gaming", bundle.getBoolean("gaming"));
                hobbies.put("music", bundle.getBoolean("music"));
                data.put("hobbies", hobbies);
                data.put("sync", bundle.getBoolean("sync"));

            } catch (JSONException e) {
                e.printStackTrace();
            }

            string = data.toString();
            try {
                fos = getContext().openFileOutput(filename, Context.MODE_PRIVATE);
                fos.write(string.getBytes());
                fos.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            String text = filename + " téléchargé(s) !";
            Toast toast = Toast.makeText(getContext(), text, Toast.LENGTH_SHORT);
            toast.show();
            getActivity().onBackPressed();
        });
    }


    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.affichage_fragment, container, false);
    }

    @Override
    public void onViewCreated(@NonNull View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        Bundle bundle = this.getArguments();
        addListenerOnButton(bundle);
        tv = getView().findViewById(R.id.affichage);

        String surname = bundle.getString("surname");
        String firstName = bundle.getString("firstName");
        String age = bundle.getString("age");
        String phoneNumber = bundle.getString("phoneNumber");
        String mailAddress = bundle.getString("mailAddress");

        Boolean sport = bundle.getBoolean("sport");
        Boolean music = bundle.getBoolean("music");
        Boolean gaming = bundle.getBoolean("gaming");
        Boolean sync = bundle.getBoolean("sync");

        String hobbiesSentence = "Vous aimez";
        if(sport) hobbiesSentence += " le sport";
        if(music) hobbiesSentence += " la musique";
        if(gaming) hobbiesSentence += " le jeu vidéo";

        String message = "Vous êtes " + surname + " " + firstName + "\n" +
                "Aujourd'hui vous avez " + age + " ans. \n" +
                hobbiesSentence + "\n" +
                "Votre numéro de téléphone est le : " + phoneNumber + ". \n" +
                "Nous pouvons vous contacter sur le mail : " + mailAddress + "\n\n" +
                (sync ? "Vous souhaitez synchroniser vos donneés" : "Vous ne souhaitez pas synchroniser vos donneés");
        tv.setText(message);
    }

}
