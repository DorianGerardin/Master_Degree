package com.dovian.tp3;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;

import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

public class SaisieFragment extends Fragment {

    public void addListenerOnButton(){
        Button buttonCall = getView().findViewById(R.id.submit);
        buttonCall.setOnClickListener(view -> {

            FragmentManager fragmentManager = getParentFragmentManager();
            FragmentTransaction fragmentTransaction = fragmentManager.beginTransaction();

            AffichageFragment affichageFragment = new AffichageFragment();
            fragmentTransaction.replace(R.id.fragment, affichageFragment, null);
            fragmentTransaction.commit();

            /*EditText nameEdit = findViewById(R.id.nomEdit);
            EditText surnameEdit = findViewById(R.id.prenomEdit);
            EditText ageEdit = findViewById(R.id.AgeEdit);
            EditText skillEdit = findViewById(R.id.SkillEdit);
            EditText phoneEdit = findViewById(R.id.phoneEdit);

            intent.putExtra("name", nameEdit.getText().toString());
            intent.putExtra("surname", surnameEdit.getText().toString());
            intent.putExtra("age", ageEdit.getText().toString());
            intent.putExtra("skill", skillEdit.getText().toString());
            intent.putExtra("phone", phoneEdit.getText().toString());*/
        });
    }

    @Override
    public void onViewCreated(View view, Bundle bundle) {
        addListenerOnButton();
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.saisie_fragment, container, false);
    }


}
