#include <stdio.h>
#include "quiz.h"

int main() {
    int choix;

    // Définition des puzzles
    Puzzle puzzles[MAX] = {
        {"Image: Chat (piece manquante)", {"Oreille", "Queue", "Patte"}, 2},
        {"Image: Ombre voiture", {"Moto", "Voiture", "Bus"}, 2},
        {"Image: Fruit coupe", {"Pomme", "Banane", "Orange"}, 1},
        {"Image: Animal ombre", {"Chien", "Chat", "Lion"}, 3},
        {"Image: Objet", {"Stylo", "Livre", "Chaise"}, 1}
    };

    do {
        // Menu principal
        printf("\n====== MENU ======\n");
        printf("1. Jouer Puzzle\n");
        printf("2. Quitter\n");
        printf("Choix: ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                jouerPuzzle(puzzles, MAX); // Appel à la fonction du quiz
                break;
            case 2:
                printf("Au revoir 👋\n");
                break;
            default:
                printf("Choix invalide\n");
        }
    } while (choix != 2);

    return 0;
}
