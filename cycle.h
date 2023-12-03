#ifndef CYCLE_H
#define CYCLE_H

typedef struct {
    int id;
    float duree;
} Operation;

typedef struct {
    int *idsOperations;
    int nombreOperations;
    float tempsTotal;
} Station;

void cycle(const char* fichierOperations, const char* fichierTempsCycle);

#endif // CYCLE_H
