#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>

#define WIDTH 20
#define HEIGHT 20

int gameOver;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum eDirection dir;

int speed = 100; // Vitesse de base (en millisecondes)
int difficulty = 1; // Niveau de difficulté (1: Facile, 2: Moyen, 3: Difficile)

void Setup() {
    gameOver = 0;
    dir = STOP;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    score = 0;
    nTail = 0;
}

void Draw() {
    clear(); // Effacer l'écran
    for (int i = 0; i < WIDTH + 2; i++)
        printw("#");
    printw("\n");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0)
                printw("#");
            if (i == y && j == x)
                printw("O"); // Tête du serpent
            else if (i == fruitY && j == fruitX)
                printw("F"); // Fruit
            else {
                int printTail = 0;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printw("o"); // Corps du serpent
                        printTail = 1;
                    }
                }
                if (!printTail)
                    printw(" ");
            }
            if (j == WIDTH - 1)
                printw("#");
        }
        printw("\n");
    }

    for (int i = 0; i < WIDTH + 2; i++)
        printw("#");
    printw("\n");
    printw("Score: %d\n", score);
    printw("Difficulty: %d\n", difficulty);
    refresh(); // Rafraîchir l'écran
}

void Input() {
    timeout(speed); // Délai pour l'entrée utilisateur
    int ch = getch();
    switch (ch) {
    case 'a':
        if (dir != RIGHT) dir = LEFT;
        break;
    case 'd':
        if (dir != LEFT) dir = RIGHT;
        break;
    case 'w':
        if (dir != DOWN) dir = UP;
        break;
    case 's':
        if (dir != UP) dir = DOWN;
        break;
    case 'x':
        gameOver = 1;
        break;
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    }

    // Gestion des bordures (téléportation)
    if (x >= WIDTH) x = 0; else if (x < 0) x = WIDTH - 1;
    if (y >= HEIGHT) y = 0; else if (y < 0) y = HEIGHT - 1;

    // Collision avec le corps
    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = 1;

    // Manger le fruit
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        nTail++;

        // Animation de clignotement
        for (int i = 0; i < 3; i++) {
            clear();
            Draw();
            refresh();
            usleep(100000); // 100 ms
        }
    }
}

void ShowMenu() {
    clear();
    printw("=== Snake Game ===\n");
    printw("1. Start Game\n");
    printw("2. Difficulty Level\n");
    printw("3. Instructions\n");
    printw("4. Exit\n");
    printw("Choose an option: ");
    refresh();
}

void ShowDifficultyMenu() {
    clear();
    printw("=== Difficulty Level ===\n");
    printw("1. Easy\n");
    printw("2. Medium\n");
    printw("3. Hard\n");
    printw("Choose a difficulty: ");
    refresh();
    int choice = getch() - '0';
    switch (choice) {
    case 1:
        difficulty = 1;
        speed = 150;
        break;
    case 2:
        difficulty = 2;
        speed = 100;
        break;
    case 3:
        difficulty = 3;
        speed = 50;
        break;
    default:
        printw("Invalid choice! Setting to Easy.\n");
        difficulty = 1;
        speed = 150;
        break;
    }
}

void ShowInstructions() {
    clear();
    printw("=== Instructions ===\n");
    printw("Use 'W' to move UP\n");
    printw("Use 'S' to move DOWN\n");
    printw("Use 'A' to move LEFT\n");
    printw("Use 'D' to move RIGHT\n");
    printw("Press 'X' to exit the game\n");
    printw("Press any key to return to the menu...");
    refresh();
    getch();
}

void GameOverScreen() {
    clear();
    printw("=== Game Over ===\n");
    printw("Your Score: %d\n", score);
    printw("1. Play Again\n");
    printw("2. Return to Menu\n");
    printw("Choose an option: ");
    refresh();
}

int main() {
    srand(time(NULL));
    initscr(); // Initialiser ncurses
    cbreak(); // Désactiver la mise en mémoire tampon des lignes
    noecho(); // Désactiver l'affichage des entrées utilisateur
    keypad(stdscr, TRUE); // Activer les touches spéciales

    int choice;
    while (1) {
        ShowMenu();
        choice = getch() - '0';
        switch (choice) {
        case 1:
            Setup();
            while (!gameOver) {
                Draw();
                Input();
                Logic();
                usleep(speed * 1000); // Ajuster la vitesse
            }
            GameOverScreen();
            choice = getch() - '0';
            if (choice == 2) break;
            break;
        case 2:
            ShowDifficultyMenu();
            break;
        case 3:
            ShowInstructions();
            break;
        case 4:
            endwin(); // Quitter ncurses
            return 0;
        default:
            printw("Invalid choice! Try again.\n");
            refresh();
            sleep(1);
            break;
        }
    }
    endwin(); // Quitter ncurses
    return 0;
}