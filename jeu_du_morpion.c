#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define SIZE 3

char board[SIZE][SIZE]; // Plateau de jeu
int currentPlayer = 1; // 1 pour Joueur 1 (X), 2 pour Joueur 2 (O)

// Initialiser le plateau
void initializeBoard() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = ' ';
        }
    }
}

// Afficher le plateau
void drawBoard() {
    clear(); // Effacer l'écran
    printw("Tic-Tac-Toe\n\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printw(" %c ", board[i][j]);
            if (j < SIZE - 1) printw("|");
        }
        printw("\n");
        if (i < SIZE - 1) printw("---+---+---\n");
    }
    printw("\nJoueur %d (%c), c'est votre tour.\n", currentPlayer, (currentPlayer == 1) ? 'X' : 'O');
    refresh();
}

// Vérifier si un joueur a gagné
char checkWin() {
    // Vérifier les lignes et colonnes
    for (int i = 0; i < SIZE; i++) {
        if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0];
        if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i];
    }
    // Vérifier les diagonales
    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];
    if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2];
    // Aucun gagnant
    return ' ';
}

// Vérifier si le plateau est plein
int isBoardFull() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == ' ')
                return 0;
        }
    }
    return 1;
}

// Jouer un tour
void playTurn() {
    int row, col;
    while (1) {
        printw("Entrez la ligne (1-3) et la colonne (1-3) : ");
        refresh();
        scanw("%d %d", &row, &col);
        row--; // Convertir en indices 0-2
        col--;

        if (row >= 0 && row < SIZE && col >= 0 && col < SIZE && board[row][col] == ' ') {
            board[row][col] = (currentPlayer == 1) ? 'X' : 'O';
            break;
        } else {
            printw("Mouvement invalide. Réessayez.\n");
            refresh();
        }
    }
}

// Fonction principale
int main() {
    initscr(); // Initialiser ncurses
    cbreak(); // Désactiver la mise en mémoire tampon des lignes
    noecho(); // Désactiver l'affichage des entrées utilisateur
    keypad(stdscr, TRUE); // Activer les touches spéciales

    initializeBoard();
    char winner = ' ';

    while (1) {
        drawBoard();
        playTurn();
        winner = checkWin();
        if (winner != ' ' || isBoardFull()) break;
        currentPlayer = (currentPlayer == 1) ? 2 : 1; // Changer de joueur
    }

    drawBoard();
    if (winner != ' ') {
        printw("Félicitations, Joueur %d (%c) a gagné !\n", currentPlayer, winner);
    } else {
        printw("Match nul !\n");
    }
    printw("Appuyez sur une touche pour quitter...\n");
    refresh();
    getch();

    endwin(); // Quitter ncurses
    return 0;
}