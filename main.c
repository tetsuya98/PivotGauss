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

int findS(int m, int n, double T[n+m+1][m+1]) { //trouve la colonne du pivot
    for (int i = 0; i < m+n; i++) {
        if (T[i][m] > 0) {
            //printf("findS : %d\n", i);
            return i;
        }
    }
    return -1;
}

int findR(int m, int n, int S, double T[n+m+1][m+1]) { //trouve la ligne du pivot
    int R[m]; //tab des indice des div min
    double min_res = 300; //res min de la div
    int nbMin = 0; //nombre de div min
    int flag = 0; //si colonne base
    int minI = n+m; //indide min, regle de bland
    
    for (int i = 0; i < m; i++) {
        printf("ligne : %d\n", i);
        printf("lignes restante : %d\n", m-i-1);
        if (T[S][i] > 0) { //si la valeur neg, pas pivotable
            printf("Z Positif : %f\n", T[S][i]);
            if (T[m+n][i] / T[S][i] < min_res) {
                min_res = T[m+n][i] / T[S][i]; //on divise la valeur de b par le valeur de la colonne
                printf("div : %f\n", min_res);
                for (int j = 0; j < m; j++) {
                    R[j] = -1;
                }
                R[0] = i;
                nbMin = 1;
            }else{
                if (T[m+n][i] / T[S][i] == min_res) {
                    R[nbMin] = i;
                    printf("indice min : %d\n", i);
                    nbMin++;
                }
            }
        }
    }
    printf("fin recherche min div\n");
    if (nbMin > 1) {
        for (int i = 0; i < nbMin; i++) {
            for (int j = 0; j < m+n; j++) {
                if (T[j][R[i]] == 1) {
                    for (int k = 0; k < m+1 && flag == 0; k++) {
                        if (T[j][k] != 0 && k != i) {
                            flag = 1;
                        }
                    }
                    if (flag == 0) {
                        if (minI > j) {
                            minI = j;
                        }
                    }
                }
            }
        }
        return minI;
    }else{
        printf("R[0] : %d\n", R[0]);
        return R[0];
    }
    
    return -1;
}

void pivoter(int S, int R, double pivot, double T[n+m+1][m+1]) {
    double alpha;
    for (int i = 0; i < m+n+1; i++) {
        T[i][R] = T[i][R] / pivot;
    }
    for (int i = 0; i < m+1; i++) {
        if (i != R) {
            alpha = T[S][i] * -1;
            for (int j = 0; j < m+n+1; j++) {
                T[j][i] = T[j][i] + alpha * T[j][R];
            }
        }
    }
    
    printf("----------Tableau----------\n");
    
    for (int i = 0; i < m+1; i++) {
        for (int j = 0; j < m+n+1; j++) {
            printf("| %f |", T[j][i]);
        }
        printf("\n");
    }
    
}

int main(int argc, const char * argv[]) {
    
    double T[n+m+1][m+1];
    int R = -2, S = -2;
    double pivot = -1;
    createT(n, m, c, a, b, T);
    
    //affichage du tableau
    printf("----------Tableau----------\n");
    for (int i = 0; i < m+1; i++) {
        for (int j = 0; j < m+n+1; j++) {
            printf("| %f |", T[j][i]);
        }
        printf("\n");
    }
    
    while (S != -1 && R != -1) {
        printf("-----------Pivot-----------\n");
        S = findS(m, n, T);
        R = findR(m, n, S, T);
        pivot = T[S][R];
        
        printf("S : %d, R : %d, pivot : %f\n", S, R, pivot);
        
        if (R != -1 && S != -1) {
            pivoter(S, R, pivot, T);
        }
    }
    
    
    
    return 0;
}
