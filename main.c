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

void createT(int n, int m, double *c, double **a, double *b, double **T, int base[m]) {
    int ind = 0;
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
                            base[ind] = i;
                            ind++;
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
//    for (int i = 0; i < m; i++) {
//        printf("base%d : %d\n", i, base[i]);
//    }
}

int findS(int m, int n, double **T) { //trouve la colonne du pivot
    for (int i = 0; i < m+n; i++) {
        if (T[i][m] > 0) {
            //printf("findS : %d\n", i);
            return i;
        }
    }
    return -1;
}

int findR(int m, int n, int S, double **T, int base[m]) { //trouve la ligne du pivot
    int R[m]; //tab des indice des div min
    double min_res = 300; //res min de la div
    int nbMin = 0; //nombre de div min
    int flag = 0; //si colonne base
    int minI = n+m; //indide min, regle de bland
    int minR = m;//
    
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
//    for (int i = 0; i < m; i++){
//        printf("R%d : %d  --> nbMin : %d\n", i, R[i], nbMin);
//    }
    printf("fin recherche min div\n");
    for (int i = 0; i < nbMin; i++) {
        for (int j = 0; j < m+n; j++) {
            if (T[j][R[i]] == 1) {
                flag = 0;
                for (int k = 0; k < m+1 && flag == 0; k++) {
                    if (T[j][k] != 0 && k != R[i]) {
                        flag = 1;
                    }
                }
                if (flag == 0) {
                    if (minI > j) {
                        minI = j;
                        minR = R[i];
                    }
                }
            }
        }
    }
    if (minR == m){
        return -1;
    }
    for (int i = 0; i < m; i++) {
        if (minI == base[i]) {
            base[i] = S;
        }
    }
    return minR;

}

void pivoter(int S, int R, int m, int n, double pivot, double **T) {
    double alpha;
    
    for (int i = 0; i < m+n+1; i++) {
        T[i][R] = T[i][R] / pivot;
    }
//    printf("----------Div par pivot----------\n");
//
//    for (int i = 0; i < m+1; i++) {
//        for (int j = 0; j < m+n+1; j++) {
//            printf("| %f |", T[j][i]);
//        }
//        printf("\n");
//    }
    for (int i = 0; i < m+1; i++) {
        if (i != R) {
            alpha = T[S][i] * (-1.0);
            //printf("alpha : %f\n", alpha);
            for (int j = 0; j < m+n+1; j++) {
                //printf("pivote : %f + %f * %f = ", T[j][i], alpha, T[j][R]);
                T[j][i] = T[j][i] + (alpha * T[j][R]);
               // printf("%f\n", T[j][i]);
            }
        }
    }
    
    printf("----------après entré dans base----------\n");

    for (int i = 0; i < m+1; i++) {
        for (int j = 0; j < m+n+1; j++) {
            printf("| %f |", T[j][i]);
        }
        printf("\n");
    }
    
}

int main(int argc, const char * argv[]) {
    
    //Valeurs exemple
    //int n = 3;
    //int m = 2;
    //int p;
    //double c[] = {3.0, 1.0, 2.0};
    //double a[2][3] = {{1.0, 0.0, 4.0},{-2.0, 1.0, -5.0}};
    //double b[] = {8.0, 10.0};

    //Random des valeurs
    srand(time(NULL));
    int n = rand() % (100000 - 1000) + 1000; //Variables de décision
    int m = rand() % (100 - 50) + 50; //Nombre de contraintes
    //int p = rand() % (2000 - 500) + 500; //Nombre de programme linéaire
    
    double c[n]; //Vecteur de profit
    double b[m]; // Resultat
    double ** a = malloc(m * sizeof(double)); //Contraintes
    double ** T = malloc((n + m + 1) * sizeof(double)); //Matrice
    
    for (int i = 0; i < m; m++) {
        a[i] = malloc(n * sizeof(double));
    }
    
    for (int i = 0; i < n; i++) {
        c[i] = rand() % 500;
    }
    
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = rand() % 100;
        }
        b[i] = rand() % (250 - 25) + 25;
    }
    
    for (int i = 0; i < n + m + 1; i++) {
        T[i] = malloc((m + 1) * sizeof(double));
    }
    
    //Init des variables
    int base[m]; //Indice de la base
    int R = -2, S = -2; //Colonne et ligne du pivot
    double pivot = -1; //Valeur du pivot
    double opti; //Solution optimale
    
    //création de la matric
    createT(n, m, c, a, b, T, base);
    
    //affichage du tableau
    printf("----------Tableau----------\n");
    for (int i = 0; i < m+1; i++) {
        for (int j = 0; j < m+n+1; j++) {
            printf("| %f |", T[j][i]);
        }
        printf("\n");
    }
    
    while (S != -1 && R != -1) {
        //Trouver le pivot
        printf("-----------Pivot-----------\n");
        S = findS(m, n, T);
        R = findR(m, n, S, T, base);
        for (int i = 0; i < m; i++) {
            printf("nouvelle base%d : %d\n", i, base[i]);
        }
        pivot = T[S][R];
        
        printf("S : %d, R : %d, pivot : %f\n", S, R, pivot);
        
        //Pivote
        if (R != -1 && S != -1) {
            pivoter(S, R, m, n, pivot, T);
        }
        
        //Calcul de la soltion optimal
        opti = 0;
        for (int i = 0; i < m; i++) {
            opti = opti + c[base[i]] * T[n+m][i];
        }
        
        printf("Solution Optimale : %f\n", opti);
    }
    
    
    
    return 0;
}
