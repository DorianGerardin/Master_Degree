#include <stdio.h>
#include <cmath>
#include <iostream>
#include "image_ppm.h"

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgLue2[250], cNomImgEcrite[250];
  int nH, nW, nTaille, nR, nG, nB;
  
  if (argc != 4) 
     {
       printf("Usage: ImageIn.ppm ImageSeuil.pgm ImageOut.ppm \n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgLue2) ;
   sscanf (argv[3],"%s",cNomImgEcrite);

   OCTET *ImgIn, *ImgIn2, *ImgOut;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   int nTaille3 = nTaille * 3;
   allocation_tableau(ImgIn, OCTET, nTaille3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgIn2, OCTET, nTaille);
   lire_image_pgm(cNomImgLue2, ImgIn2, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille3);


   OCTET R[nTaille];
   OCTET G[nTaille];
   OCTET B[nTaille];

   OCTET newR[nTaille];
   OCTET newG[nTaille];
   OCTET newB[nTaille];

   for(int i=0;i<nTaille3;i+=3){
    R[i/3]=ImgIn[i];
    G[i/3]=ImgIn[i+1];
    B[i/3]=ImgIn[i+2];
   }


   int count=0;
 for (int i=0; i < nH; i++)
   for (int j=0; j < nW; j++)
     {
      int k=1;
      float moy=0.0;
      moy+=R[i*nW+j];
      
        count++;
        std::cout<<count<<std::endl;
          if(i-1>0){
            k++;
            moy+=R[(i-1)*nW+j];
            if(j-1>0){
              k++;
              moy+=R[(i-1)*nW+j-1];
            }
            if(j+1<nW){
              k++;
              moy+=R[(i-1)*nW+j+1];
            }
          }
          if(j-1>0){
            k++;
            moy+=R[i*nW+j-1];
          }
          if(j+1<nW){
            k++;
            moy+=R[i*nW+j+1];
          }
          if(i+1<nH){
            k++;
            moy+=R[(i+1)*nW+j];
            if(j-1>0){
              k++;
              moy+=R[(i+1)*nW+j-1];
            }
            if(j+1<nW){
              k++;
              moy+=R[(i+1)*nW+j+1];
            }
         }
         moy/=(float)k;
         if(moy>255){
          moy=255.0;
         }
       
       newR[i*nW+j]=(int)moy;
     }

     for (int i=0; i < nH; i++)
   for (int j=0; j < nW; j++)
     {
      int k=1;
      float moy=0.0;
      moy+=G[i*nW+j];
 
        if(i-1>0){
          k++;
          moy+=G[(i-1)*nW+j];
          if(j-1>0){
            k++;
            moy+=G[(i-1)*nW+j-1];
          }
          if(j+1<nW){
            k++;
            moy+=G[(i-1)*nW+j+1];
          }
        }
        if(j-1>0){
          k++;
          moy+=G[i*nW+j-1];
        }
        if(j+1<nW){
          k++;
          moy+=G[i*nW+j+1];
        }
        if(i+1<nH){
          k++;
          moy+=G[(i+1)*nW+j];
          if(j-1>0){
            k++;
            moy+=G[(i+1)*nW+j-1];
          }
          if(j+1<nW){
            k++;
            moy+=G[(i+1)*nW+j+1];
          }
       }
       moy/=(float)k;
       if(moy>255){
        moy=255.0;
       }
     
       newG[i*nW+j]=(int)moy;
     }

     for (int i=0; i < nH; i++)
   for (int j=0; j < nW; j++)
     {
      int k=1;
      float moy=0.0;
      moy+=B[i*nW+j];
  
        if(i-1>0){
          k++;
          moy+=B[(i-1)*nW+j];
          if(j-1>0){
            k++;
            moy+=B[(i-1)*nW+j-1];
          }
          if(j+1<nW){
            k++;
            moy+=B[(i-1)*nW+j+1];
          }
        }
        if(j-1>0){
          k++;
          moy+=B[i*nW+j-1];
        }
        if(j+1<nW){
          k++;
          moy+=B[i*nW+j+1];
        }
        if(i+1<nH){
          k++;
          moy+=B[(i+1)*nW+j];
          if(j-1>0){
            k++;
            moy+=B[(i+1)*nW+j-1];
          }
          if(j+1<nW){
            k++;
            moy+=B[(i+1)*nW+j+1];
          }
       }
       moy/=(float)k;
       if(moy>255){
        moy=255.0;
       }
     
       newB[i*nW+j]=(int)moy;
     }

     for(int i=0;i<nTaille3;i+=3){
      ImgOut[i]=newR[i/3];
      ImgOut[i+1]=newG[i/3];
      ImgOut[i+2]=newB[i/3];
     }

   ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}
