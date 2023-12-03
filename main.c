// main.c
#include "exclusion.h"
#include "cycle.h"
#include "precedence.h"

int main() {
    exclusion("exclusions.txt");
    cycle("tempsOperations.txt", "cycle.txt");
    precedence("Precedences.txt");
    return 0;
}
