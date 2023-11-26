#ifndef PRECEDENCES_H
#define PRECEDENCES_H

typedef struct Noeud {
    int destination;
    struct Noeud* suivant;
} Noeud;

typedef struct {
    int cle;
    int valeur;
} Mapping;

void initialiserMappage();
void libererMappage();
void ajouterMappage(int cle, int valeur);
int obtenirIndice(int cle);
Noeud* creerNoeud(int destination);
void ajouterArc(Noeud** graphe, int depart, int arrivee);
void triTopologique(Noeud** graphe, int* degreEntrant, int nombreNoeuds);
void effectuerTriTopologiqueDepuisFichier(const char* nomFichier);

#endif // PRECEDENCE_H
