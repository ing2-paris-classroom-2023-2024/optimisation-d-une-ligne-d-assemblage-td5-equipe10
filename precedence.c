#include "precedence.h"
#include <stdio.h>
#include <stdlib.h>

Mapping* mappage;
int tailleMappage = 0;
int capaciteMappage = 10;

void initialiserMappage() {
    mappage = (Mapping*) malloc(capaciteMappage * sizeof(Mapping));
    for (int i = 0; i < capaciteMappage; ++i) {
        mappage[i].cle = -1;  // Initialise les clés à une valeur non utilisée
        mappage[i].valeur = 0;
    }
}

void libererMappage() {
    free(mappage);
}

void ajouterMappage(int cle, int valeur) {
    if (tailleMappage == capaciteMappage) {
        capaciteMappage *= 2;
        mappage = (Mapping*) realloc(mappage, capaciteMappage * sizeof(Mapping));
    }
    mappage[tailleMappage].cle = cle;
    mappage[tailleMappage].valeur = valeur;
    tailleMappage++;
}

int obtenirIndice(int cle) {
    for (int i = 0; i < tailleMappage; ++i) {
        if (mappage[i].cle == cle) {
            return mappage[i].valeur;
        }
    }
    ajouterMappage(cle, tailleMappage);
    return tailleMappage - 1;
}

Noeud* creerNoeud(int destination) {
    Noeud* nouveauNoeud = (Noeud*) malloc(sizeof(Noeud));
    nouveauNoeud->destination = destination;
    nouveauNoeud->suivant = NULL;
    return nouveauNoeud;
}

void ajouterArc(Noeud** graphe, int depart, int arrivee) {
    int indexDepart = obtenirIndice(depart);
    int indexArrivee = obtenirIndice(arrivee);

    Noeud* nouveauNoeud = creerNoeud(indexArrivee);
    nouveauNoeud->suivant = graphe[indexDepart];
    graphe[indexDepart] = nouveauNoeud;
}

void triTopologique(Noeud** graphe, int* degreEntrant, int nombreNoeuds) {
    int* file = (int*) malloc(nombreNoeuds * sizeof(int));
    int debut = 0, fin = 0;
    printf("\nPrecedence:\n");

    for (int i = 0; i < nombreNoeuds; i++) {
        if (degreEntrant[i] == 0) {
            file[fin++] = i;
        }
    }

    while (debut < fin) {
        int courant = file[debut++];
        for (int i = 0; i < tailleMappage; i++) {
            if (mappage[i].valeur == courant) {
                printf("%d ", mappage[i].cle);
                break;
            }
        }

        Noeud* temp = graphe[courant];
        while (temp != NULL) {
            degreEntrant[temp->destination]--;
            if (degreEntrant[temp->destination] == 0) {
                file[fin++] = temp->destination;
            }
            temp = temp->suivant;
        }
    }
    printf("\n");

    free(file);
}

void effectuerTriTopologiqueDepuisFichier(const char* nomFichier) {
    initialiserMappage();

    FILE* fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        libererMappage();
        return;
    }

    int depart, arrivee;
    while (fscanf(fichier, "%d %d", &depart, &arrivee) != EOF) {
        obtenirIndice(depart);
        obtenirIndice(arrivee);
    }

    Noeud** graphe = (Noeud**) malloc(tailleMappage * sizeof(Noeud*));
    for (int i = 0; i < tailleMappage; i++) {
        graphe[i] = NULL;
    }

    int* degreEntrant = (int*) calloc(tailleMappage, sizeof(int));

    fseek(fichier, 0, SEEK_SET);
    while (fscanf(fichier, "%d %d", &depart, &arrivee) != EOF) {
        ajouterArc(graphe, depart, arrivee);
        degreEntrant[obtenirIndice(arrivee)]++;
    }

    fclose(fichier);

    triTopologique(graphe, degreEntrant, tailleMappage);

    for (int i = 0; i < tailleMappage; i++) {
        Noeud* temp;
        while (graphe[i] != NULL) {
            temp = graphe[i];
            graphe[i] = graphe[i]->suivant;
            free(temp);
        }
    }

    free(graphe);
    free(degreEntrant);
    libererMappage();
}
