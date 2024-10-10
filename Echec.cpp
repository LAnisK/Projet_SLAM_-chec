#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 8

// Déclaration des pièces et du plateau
char board[SIZE][SIZE];

// Initialisation des pièces : R - Tour, C - Cavalier, Q - Reine, B - Fou, . - Case vide
void initBoard() {
    // On vide le plateau
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = '.';
        }
    }

    // Position des pièces joueur 1 (bas du plateau)
    board[7][0] = 'R';  // Tour
    board[7][1] = 'C';  // Cavalier
    board[7][2] = 'B';  // Fou
    board[7][3] = 'Q';  // Reine

    // Position des pièces joueur 2 (haut du plateau)
    board[0][0] = 'r';  // Tour
    board[0][1] = 'c';  // Cavalier
    board[0][2] = 'b';  // Fou
    board[0][3] = 'q';  // Reine
}

// Afficher le plateau
void displayBoard() {
    printf("\n  a b c d e f g h\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", SIZE - i);
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Fonction de sauvegarde
void saveGame() {
    FILE *file = fopen("savegame.txt", "w");
    if (!file) {
        printf("Erreur : Impossible de sauvegarder la partie.\n");
        return;
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fprintf(file, "%c", board[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    printf("Partie sauvegardee avec succès !\n");
}

// Fonction pour charger une partie sauvegardée
void loadGame() {
    FILE *file = fopen("savegame.txt", "r");
    if (!file) {
        printf("Erreur : Impossible de charger la partie.\n");
        return;
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fscanf(file, " %c", &board[i][j]);
        }
    }
    fclose(file);
    printf("Partie chargee avec succes !\n");
}

// Vérifier si un mouvement est valide selon les règles d'échecs pour chaque pièce
int isMoveValid(char piece, int sx, int sy, int dx, int dy) {
    int deltaX = dx - sx;
    int deltaY = dy - sy;

    switch (piece) {
        case 'R':  // Tour
        case 'r':
            return (sx == dx || sy == dy);
        case 'C':  // Cavalier
        case 'c':
            return (abs(deltaX) == 2 && abs(deltaY) == 1) || (abs(deltaX) == 1 && abs(deltaY) == 2);
        case 'B':  // Fou
        case 'b':
            return abs(deltaX) == abs(deltaY);
        case 'Q':  // Reine
        case 'q':
            return (sx == dx || sy == dy || abs(deltaX) == abs(deltaY));
        default:
            return 0;
    }
}

// Déplacer une pièce
void movePiece(int sx, int sy, int dx, int dy) {
    board[dx][dy] = board[sx][sy];
    board[sx][sy] = '.';
}

// Fonction pour demander un mouvement au joueur
void playerMove(char player) {
    char sx, dx;
    int sy, dy;
    int valid = 0;

    do {
        printf("Joueur %c, entrez votre mouvement (ex: a2 a4): ", player);
        scanf(" %c%d %c%d", &sx, &sy, &dx, &dy);

        // Convertir l'entrée en index du tableau
        sx -= 'a';
        sy = SIZE - sy;
        dx -= 'a';
        dy = SIZE - dy;

        if (isMoveValid(board[sy][sx], sy, sx, dy, dx)) {
            movePiece(sy, sx, dy, dx);
            valid = 1;
        } else {
            printf("Mouvement invalide. Veuillez réessayer.\n");
        }
    } while (!valid);
}

// Mouvement aléatoire pour l'ordinateur (basique)
void computerMove() {
    int sx, sy, dx, dy;
    int valid = 0;
    
    while (!valid) {
        sx = rand() % SIZE;
        sy = rand() % SIZE;
        dx = rand() % SIZE;
        dy = rand() % SIZE;

        if (board[sy][sx] >= 'r' && isMoveValid(board[sy][sx], sy, sx, dy, dx)) {
            movePiece(sy, sx, dy, dx);
            valid = 1;
        }
    }
    printf("L'ordinateur a joue.\n");
}

// Mode Joueur contre Joueur
void playVsPlayer() {
    int turns = 0;

    while (1) {
        displayBoard(); // Afficher le plateau mis à jour d'abord
        if (turns % 2 == 0) {
            playerMove('1');
        } else {
            playerMove('2');
        }
        turns++;

        printf("Sauvegarder la partie ? (y/n) : ");
        char saveChoice;
        scanf(" %c", &saveChoice);
        if (saveChoice == 'y') {
            saveGame();
            return;
        }
    }
}

// Mode Joueur contre Ordinateur
void playVsComputer() {
    int turns = 0;

    while (1) {
        displayBoard(); // Afficher le plateau mis à jour d'abord
        if (turns % 2 == 0) {
            playerMove('1');
        } else {
            computerMove();
        }
        turns++;

        printf("Sauvegarder la partie ? (y/n) : ");
        char saveChoice;
        scanf(" %c", &saveChoice);
        if (saveChoice == 'y') {
            saveGame();
            return;
        }
    }
}

// Menu principal
int main() {
    int choice;
    srand(time(NULL));

    printf("Bienvenue au jeu d'echecs !\n");
    printf("1. Joueur contre Joueur\n");
    printf("2. Joueur contre Ordinateur\n");
    printf("3. Charger une partie\n");
    printf("Entrez votre choix : ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            initBoard();
            playVsPlayer();
            break;
        case 2:
            initBoard();
            playVsComputer();
            break;
        case 3:
            loadGame();
            printf("Partie chargee :\n");
            displayBoard();
            printf("1. Continuer contre Joueur\n2. Continuer contre Ordinateur\n");
            int resumeChoice;
            scanf("%d", &resumeChoice);
            if (resumeChoice == 1) {
                playVsPlayer();
            } else {
                playVsComputer();
            }
            break;
        default:
            printf("Choix invalide.\n");
            break;
    }

    return 0;
}

