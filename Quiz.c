#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "quiz.h"

// Affiche le message avant chaque niveau
void messageStage(int niveau) {
    printf("\n=============================\n");
    printf("🎯 Niveau %d\n", niveau);
    printf("Choisissez la bonne piece !\n");
    printf("=============================\n");
}

// Affiche le résultat après chaque réponse
void messageResultat(int correct) {
    if (correct)
        printf("✅ Bravo ! Bonne réponse\n");
    else
        printf("❌ Mauvaise réponse\n");
}

// Fonction principale pour jouer au quiz
void jouerPuzzle(Puzzle p[], int n) {
    int score = 0;
    int vies = 3;
    int utilise[MAX] = {0}; // pour éviter les questions répétées
    int niveau = 1;

    srand(time(NULL));

    while (vies > 0) {
        int index;

        // Choisir une question non utilisée
        do {
            index = rand() % n;
        } while (utilise[index] == 1);

        utilise[index] = 1;

        // Afficher le niveau
        messageStage(niveau);

        // Afficher la question et les choix
        printf("%s\n", p[index].question);
        for (int i = 0; i < 3; i++)
            printf("%d. %s\n", i + 1, p[index].choix[i]);

        // Gestion du timer
        time_t start = time(NULL);
        int rep;
        printf("Votre choix (10 sec): ");
        scanf("%d", &rep);
        time_t end = time(NULL);

        if (difftime(end, start) > 10) {
            printf("⏱️ Temps écoulé !\n");
            vies--;
        } else if (rep == p[index].reponse) {
            messageResultat(1);
            score += 10;
            niveau++;
        } else {
            messageResultat(0);
            vies--;
        }

        printf("Score: %d | Vies: %d\n", score, vies);
    }

    printf("\n💀 Game Over\nScore final: %d\n", score);
}
