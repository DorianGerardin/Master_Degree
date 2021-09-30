#include "eval-perf.cpp"
#include "test.h"
#include <iostream>
#include <string>
#include <vector>

const int N = 100000000;

void afficheTab(std::vector<int> tab) {
  std::string tabString = "{ ";
  int size = tab.size();
  for(int i = 0; i < size; i++) {
    tabString = tabString + std::to_string(tab[i]) + ", ";
  }
  tabString = tabString.substr(0, tabString.length() - 2);
  tabString = tabString + " }";
  std::cout << tabString << std::endl;
}


//---------------------------------EX 4---------------------------------//


void sommePrefixe(std::vector<int> &tab) {
  int size = tab.size();
  for(int i = 1; i < size; i++) {
    tab[i] += tab[i-1];
  }
}

void generateVector(std::vector<std::vector<int>> &tab, int n, int min, int max) {
    int len;
    for(int i = 0; i < n; i++) {
        std::vector<int> this_v;
        this_v.clear();
        len = rand()%(max-min + 1) + min;
        for(int j = 0; j < len; j++) {
            this_v.push_back(rand() % 100 + 1);
        }
        tab.push_back(this_v);
    }
}


//---------------------------------EX 5---------------------------------//


int polyEval(std::vector<int> coeffs) {
    int alpha = coeffs[0];
    int result = coeffs[1];
    int thisAlpha = coeffs[0];
    for(int i = 2; i < coeffs.size(); i++) {
        result += coeffs[i]*thisAlpha;
        thisAlpha *= alpha;
    }
    return result;
}

int polyEvalHorner(std::vector<int> coeffs) {
    int alpha = coeffs[0];
    int result = 0;
    int size = coeffs.size();
    for(int i = 1; i < size; i++) {
        result = result * alpha + coeffs[size-i];
    }
    return result;
}


//---------------------------------EX 7---------------------------------//

    


int main (int argc, char *argv[]){


    //------Ex 4-------//

    // if(argc != 2) {
    //     printf("Il faut un argument \n");
    //     exit(0);
    // }

//   int nbAppel = atoi(argv[1]);
//   std::vector<std::vector<int>> vect_tab;
//   generateVector(vect_tab, nbAppel, 10000, 10000);

    //------Ex 5-------//

    std::vector<int> coeffs;

    for(int i = 1; i < argc; i++) {
        coeffs.push_back(atoi(argv[i]));
    }

  EvalPerf PE;
  PE.start() ;

    //------Ex 4-------//

//   for(int i = 0; i < nbAppel; i++) {
//     //afficheTab(vect_tab[i]);
//     sommePrefixe(vect_tab[i]);
//   }

    //------Ex 5-------//

  int result = polyEval(coeffs);
  int resultHorner = polyEvalHorner(coeffs);
  
  PE.stop() ;

  //afficheTab(tab);
  std::cout<<"poly Eval : " << result <<std::endl ;
  std::cout<<"poly Eval Horner : " << resultHorner <<std::endl ;

  std::cout<<"nbr cycles : " << PE.nb_cycles()<<std::endl ;
  std::cout<<"nbr secondes : " << PE.second().count()<<std::endl ;
  std::cout<<"nbr millisecondes : "<< PE.milisecond().count()<<std::endl ;
  std::cout<<"CPI = "<< PE.CPI(N)<<std::endl ;


 return 0;

}