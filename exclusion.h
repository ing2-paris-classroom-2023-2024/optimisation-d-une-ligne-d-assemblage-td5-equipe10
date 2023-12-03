// exclusion.h
#ifndef EXCLUSION_H
#define EXCLUSION_H

typedef struct {
    int operation1;
    int operation2;
} PaireExclusion;

typedef struct {
    int operation;
    int degre;
} Sommet;

int lirePairesExclusion(const char *nomFichier, PaireExclusion **paires);
void trierSommetsParDegre(Sommet *sommets, int nombreSommets, PaireExclusion *paires, int compteurPaires);
void colorerGrapheWelshPowell(PaireExclusion *paires, int compteurPaires, Sommet *sommets, int nombreSommets, int **couleur);
void afficherStations(int *couleur, int maxOp);
void exclusion(const char *nomFichier);

#endif // EXCLUSION_H
