#include "cycle.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int comparerOperations(const void *a, const void *b) {
    Operation *opA = (Operation *)a;
    Operation *opB = (Operation *)b;
    return (opB->duree > opA->duree) - (opB->duree < opA->duree);
}

float lireTempsCycle(const char* nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (!fichier) {
        perror("Erreur lors de l'ouverture du fichier de temps de cycle");
        exit(EXIT_FAILURE);
    }

    float tempsCycle;
    fscanf(fichier, "%f", &tempsCycle);
    fclose(fichier);
    return tempsCycle;
}

Operation* lireOperations(const char* nomFichier, int *compte) {
    FILE *fichier = fopen(nomFichier, "r");
    if (!fichier) {
        perror("Erreur lors de l'ouverture du fichier des opérations");
        exit(EXIT_FAILURE);
    }

    Operation *operations = malloc(sizeof(Operation) * 100);
    int capacite = 100, i = 0;

    while (fscanf(fichier, "%d %f", &operations[i].id, &operations[i].duree) != EOF) {
        i++;
        if (i >= capacite) {
            capacite *= 2;
            operations = realloc(operations, sizeof(Operation) * capacite);
        }
    }

    *compte = i;
    fclose(fichier);
    return operations;
}

void assignerOperationsAuxStations(Operation *operations, int compte, float tempsCycle) {
    qsort(operations, compte, sizeof(Operation), comparerOperations);

    Station *stations = calloc(compte, sizeof(Station));
    int nombreStations = 0;

    for (int i = 0; i < compte; i++) {
        bool assignee = false;
        for (int j = 0; j < nombreStations; j++) {
            if (stations[j].tempsTotal + operations[i].duree <= tempsCycle) {
                stations[j].idsOperations = realloc(stations[j].idsOperations, (stations[j].nombreOperations + 1) * sizeof(int));
                stations[j].idsOperations[stations[j].nombreOperations++] = operations[i].id;
                stations[j].tempsTotal += operations[i].duree;
                assignee = true;
                break;
            }
        }

        if (!assignee) {
            stations[nombreStations].idsOperations = malloc(sizeof(int));
            stations[nombreStations].idsOperations[0] = operations[i].id;
            stations[nombreStations].nombreOperations = 1;
            stations[nombreStations].tempsTotal = operations[i].duree;
            nombreStations++;
        }
    }
    printf("\nCycle et operations assignees aux stations:");
    for (int i = 0; i < nombreStations; i++) {
        printf("\nStation %d: ", i + 1);
        for (int j = 0; j < stations[i].nombreOperations; j++) {
            printf("Operation %d ", stations[i].idsOperations[j]);
            if (j < stations[i].nombreOperations - 1) printf(", ");
        }
        printf("\n");
        free(stations[i].idsOperations);
    }

    free(stations);
}

void traiterLignesAssemblage(const char* fichierOperations, const char* fichierTempsCycle) {
    int compte;
    Operation *operations = lireOperations(fichierOperations, &compte);
    float tempsCycle = lireTempsCycle(fichierTempsCycle);

    assignerOperationsAuxStations(operations, compte, tempsCycle);

    free(operations);
}
