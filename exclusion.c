// exclusion.c
#include "exclusion.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int lirePairesExclusion(const char *nomFichier, PaireExclusion **paires) {
    FILE *fichier = fopen(nomFichier, "r");
    if (!fichier) {
        perror("Erreur lors de l'ouverture du fichier");
        return -1;
    }

    int capacite = 10, compteurPaires = 0;
    *paires = malloc(capacite * sizeof(PaireExclusion));

    while (fscanf(fichier, "%d %d", &(*paires)[compteurPaires].operation1, &(*paires)[compteurPaires].operation2) == 2) {
        compteurPaires++;
        if (compteurPaires == capacite) {
            capacite *= 2;
            *paires = realloc(*paires, capacite * sizeof(PaireExclusion));
        }
    }

    fclose(fichier);
    return compteurPaires;
}

int comparerSommets(const void *a, const void *b) {
    Sommet *sommetA = (Sommet *)a;
    Sommet *sommetB = (Sommet *)b;
    return sommetB->degre - sommetA->degre;
}

void trierSommetsParDegre(Sommet *sommets, int nombreSommets, PaireExclusion *paires, int compteurPaires) {
    for (int i = 0; i < compteurPaires; i++) {
        sommets[paires[i].operation1].degre++;
        sommets[paires[i].operation2].degre++;
    }

    qsort(sommets, nombreSommets + 1, sizeof(Sommet), comparerSommets);
}

void colorerGrapheWelshPowell(PaireExclusion *paires, int compteurPaires, Sommet *sommets, int nombreSommets, int **couleur) {
    bool *disponible = malloc((nombreSommets + 1) * sizeof(bool));

    for (int i = 0; i <= nombreSommets; i++) {
        (*couleur)[sommets[i].operation] = -1;
        disponible[i] = true;
    }

    (*couleur)[sommets[0].operation] = 0;

    for (int i = 1; i <= nombreSommets; i++) {
        int op = sommets[i].operation;

        for (int j = 0; j <= nombreSommets; j++) {
            disponible[j] = true;
        }

        for (int j = 0; j < compteurPaires; j++) {
            if (paires[j].operation1 == op || paires[j].operation2 == op) {
                if ((*couleur)[paires[j].operation1] != -1)
                    disponible[(*couleur)[paires[j].operation1]] = false;
                if ((*couleur)[paires[j].operation2] != -1)
                    disponible[(*couleur)[paires[j].operation2]] = false;
            }
        }

        for (int clr = 0; clr <= nombreSommets; clr++) {
            if (disponible[clr]) {
                (*couleur)[op] = clr;
                break;
            }
        }
    }

    free(disponible);
}

void afficherStations(int *couleur, int maxOp) {
    int nombreStations = 0;
    for (int i = 1; i <= maxOp; i++) {
        if (couleur[i] + 1 > nombreStations) {
            nombreStations = couleur[i] + 1;
        }
    }

    int **stations = malloc(nombreStations * sizeof(int *));
    int *tailleStations = calloc(nombreStations, sizeof(int));

    for (int i = 0; i < nombreStations; i++) {
        stations[i] = malloc(maxOp * sizeof(int));
        tailleStations[i] = 0;
    }

    for (int i = 1; i <= maxOp; i++) {
        int station = couleur[i];
        stations[station][tailleStations[station]++] = i;
    }

    for (int i = 0; i < nombreStations; i++) {
        printf("\nStation%d: ", i + 1);
        for (int j = 0; j < tailleStations[i]; j++) {
            printf("Operation %d", stations[i][j]);
            if (j < tailleStations[i] - 1) printf(", ");
        }
        printf("\n");
        free(stations[i]);
    }

    free(stations);
    free(tailleStations);
}

void optimiserChaineAssemblage(const char *nomFichier) {
    PaireExclusion *paires;
    int compteurPaires = lirePairesExclusion(nomFichier, &paires);

    if (compteurPaires > 0) {
        int maxOp = 0;
        for (int i = 0; i < compteurPaires; i++) {
            if (paires[i].operation1 > maxOp) maxOp = paires[i].operation1;
            if (paires[i].operation2 > maxOp) maxOp = paires[i].operation2;
        }

        Sommet *sommets = malloc((maxOp + 1) * sizeof(Sommet));
        for (int i = 0; i <= maxOp; i++) {
            sommets[i].operation = i;
            sommets[i].degre = 0;
        }

        trierSommetsParDegre(sommets, maxOp, paires, compteurPaires);

        int *couleur = malloc((maxOp + 1) * sizeof(int));
        colorerGrapheWelshPowell(paires, compteurPaires, sommets, maxOp, &couleur);

        afficherStations(couleur, maxOp);

        free(couleur);
        free(sommets);
        free(paires);
    } else {
        printf("Aucune paire d'exclusion à traiter ou erreur de fichier.\n");
    }
}
