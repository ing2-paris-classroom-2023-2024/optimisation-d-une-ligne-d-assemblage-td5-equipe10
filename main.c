// main.c
#include "exclusion.h"
#include "cycle.h"
#include "precedence.h"

int main() {
    optimiserChaineAssemblage("exclusions.txt");
    traiterLignesAssemblage("tempsOperations.txt", "cycle.txt");
    effectuerTriTopologiqueDepuisFichier("Precedences.txt");
    return 0;
}
