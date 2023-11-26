// main.c
#include "exclusion.h"
#include "cycle.h"

int main() {
    optimiserChaineAssemblage("exclusions.txt");
    traiterLignesAssemblage("tempsOperations.txt", "cycle.txt");
    return 0;
}
