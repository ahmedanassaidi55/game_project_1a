#ifndef QUIZ_H
#define QUIZ_H

#define MAX 5

// Structure pour une question du quiz
typedef struct {
    char question[100];
    char choix[3][50];
    int reponse;  // numéro du choix correct (1, 2 ou 3)
} Puzzle;

// Déclarations des fonctions
void messageStage(int niveau);
void messageResultat(int correct);
void jouerPuzzle(Puzzle p[], int n);

#endif
