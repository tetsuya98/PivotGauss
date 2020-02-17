//
//  main.c
//  ADE
//
//  Created by Josselin on 06/02/2020.
//  Copyright © 2020 Josselin. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
//#include <windows.h>

//Init variables
int n = 3; /*variable de décision*/
int m = 2; /*nb de contraintes*/ //m <= n
//n nb alea [1000; 100 000]; m nb alea [50; 1000]
int p; //nb alea [500; 2000]
double c[] = {3.0, 1.0, 2.0}; //vecteur de profit [0; 500]
double a[2][3] = {{1.0, 0.0, 4.0}, {-2.0, 1.0, -5.0}}; //matrice de contrainte dans [0; 100]
double b[] = {8.0, 10.0}; //resultat dans [25; 250]

void createT(int n, int m, double c[n], double a[m][n], double b[n], double T[n+m+1][m+1]) {
    for (int i = 0; i < n+m+1; i++) {
        for (int j = 0; j < m+1; j++) {
            if (j == m) { //ligne Z
                if (i >= n) { //B et base
                    T[i][j] = 0.0;
                }else{
                    T[i][j] = c[i];
                }
            }else{
                if (i >= n) { //base et B
                    if (i == n+m) { //B
                        T[i][j] = b[j];
                    }else{ //base
                        if (i == j+n) {
                            T[i][j] = 1.0;
                        }else {
                            T[i][j] = 0.0;
                        }
                    }
                }else{ //hors base
                    T[i][j] = a[j][i];
                }
            }
        }
    }
}



int main(int argc, const char * argv[]) {
    
    double T[n+m+1][m+1];
    createT(n, m, c, a, b, T);
    
    
    
    //affichage du tableau
    for (int i = 0; i < m+1; i++) {
        for (int j = 0; j < m+n+1; j++) {
            printf("| %f |", T[j][i]);
        }
        printf("\n");
    }
    return 0;
}
