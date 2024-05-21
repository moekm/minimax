#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

/**
 * Simple Tic Tac Toe game using the MiniMax Algorithm 
 * TODO:
 * - Add configuration options (custom symbols - debug display - etc..)
 * - Add row column number support A1 - B3 - C2
 * - Add GUI Support?
*/

   /**
     * Drawing the grid in the terminal (NO GUI SUPPORT YET)
     *     1   2   3
     * A | O | X | O |    
     * B | X | X | O |
     * C | X | O | X |
    */

typedef struct {
    int index;
    int score;
} Moves;

void update();                              // updates the grid
int  status();                              // checks for wins 
bool selected(int input);                   // checks if the square is already selected
void plyturn(int round, int *player);       // calcuates the player turns from the round
void computerPlay(int mode, int player[]);  // will assain what bot to play against
void randoBot();                            // a bot that you can play against
Moves miniMax(int player);  
void playMax(int playerGridID);

enum MSGID { SELECTED , NUMBERS_ONLY, SELECT_MODE, INVALID_MODE, WIN , DRAW , DRAW_BOT, MEMORY_ERR};
enum MODES { Human = 1 , EasyBot, MaxBot };
enum PLAYERS_SYMBOLS_GRID_ID { X = -1 , O = 1};


   // Globals
   int  grid[9] = {0,0,0,0,0,0,0,0,0};
   char symbols[2] = {'X', 'O'};

    char MSG[][100] = {
    "\nThe square has already been selected.",
    "\nValid inputs are between 1 to 9",
    "\n1- Offline [2 Players]\n2- Vs Bot 1 [Easy]\n3- Vs MiniMax [Hard]\nSelect Mode: ",
    "\nMode Not Found! Please Select (1 - 2 - 3)",
    "\nYay! %c HAS WON!",
    "\nDRAW! Maybe try using both hands?",
    "DRAW! Bot: Hmm.. you cheated?",
    "Error: Memory Allocation Failed!"
   };

int main() {

   int input, mode, player[2]; // player = {playerID (for the grid), playerDisplayNumber}
   int round, winner;


   // Select Play Mode
   while(true){
    printf("%s", MSG[SELECT_MODE]);

    if (scanf("%d", &input) != 1){
     getchar(); // consume '\n'
     printf("%s\n", MSG[INVALID_MODE]);
     continue;
    }

    if (input > 3  || input <= 0){
     printf("%s\n", MSG[INVALID_MODE]);
     continue;
    }

    mode = input;
    break;
    }

    // initialize grid
    update(); 

    while (round < 9) {
        plyturn(round, player); // check who's turn it is
       
        while (true) {
            if (mode != Human && player[1] == 2){ // will play as player 2
                computerPlay(mode, player);       // Reminder: Change this later to make it more dynamic :O
                update();
                break;
            } 

            printf("\nPlayer (%d):", player[1]);

            if (scanf("%d", &input) != 1){
            getchar(); // consume '\n'
            printf(MSG[NUMBERS_ONLY]);
            continue;
            } else if (input > 9 || input <= 0){
                printf(MSG[NUMBERS_ONLY]);
                continue;
            }

            if (selected(input)){
                update();
                printf("\n%s", MSG[SELECTED]);
                continue;
            } 

                // update the grid value
                grid[input - 1] = player[0]; 
                update();
                break;
        }

        winner = status(); 
        if ((winner == X || winner == O) || round == 8) break; 

        // printf("\nround: %d", round);
        round++;
    }
    
    if (winner == -1) printf(MSG[WIN], symbols[0]);
    if (winner == 1) printf(MSG[WIN], symbols[1]);
    if (winner == 0) printf(mode != Human ? MSG[DRAW_BOT] : MSG[DRAW]);

    printf("\n");
    system("pause");

    return 0;
}

void update(){

   // clears previous logs
   #ifdef _WIN32
        system("cls");
    #else
        // ANSI Support for (Linux and macOS)
        printf("\033[H\033[J");
    #endif

    for (int i=1; i <= 3; i++){
        printf("%s", (i == 1) ? "  " : ""); // extra padding for the first element 
        printf("   %d", i);
    }

    for (int i=0; i < 3; i++){
        printf("\n%c", i + 65); // A B C
        printf ("  |");

        // printing the tic tac toe values
        for (int j= (i * 3) ; j < (i * 3) + 3 ; j++){

            char render;
            switch(grid[j]){
                case -1: // X
                    render = symbols[0];
                break;
                case 1: // O
                    render = symbols[1];
                break;
                default:
                    render = ' ';
                break;
            }

            printf(" %c |", render);
        }
    }

}

bool selected(int input){
    if (grid[input - 1] == -1 || grid[input - 1] == 1){
        return true; // true
    }
}

int status(){

    int player = 0;
    for (int i = 0; i < 3; i++){
        // checks for column wins
        if (grid[i] == grid[i + 3] && grid[i] == grid[i + 6] && grid[i] != 0){
            player = grid[i];
        } 
        // checks for row wins
        if (grid[i * 3] == grid[(i * 3) + 1] && grid[i * 3] == grid[(i * 3) + 2] && grid[i * 3] != 0){
            player = grid[i * 3];
        } 
    }

    // check for diagonals
    if (grid[0] == grid[4] && grid[0] == grid[8] && grid[0] != 0){
        player = grid[0];
    }
     if (grid[2] == grid[4] && grid[2] == grid[6] && grid[2] != 0){
        player = grid[2];
    }

    return player;
}

void plyturn(int round, int *player){

    round %= 2;
    if (round == 1){
        *(player) = 1; // O
        *(player + 1) = 2; 
    } 
    
      if (round == 0){
        *(player) = -1; // X
        *(player + 1) = 1; 
    } 
}

void computerPlay(int mode, int player[]){

switch (mode) {
    case EasyBot:
    randoBot();
    break;

    case MaxBot:
    playMax(player[0]);
    break;
}
}

void randoBot(){

     int botPosition;
     srand(time(0)); // seed
    
        do {
        botPosition = rand() % 10; // upto 9
        }
        while(selected(botPosition));

        grid[botPosition - 1] = 1;
       
}

Moves miniMax(int player) {
    
    Moves bestMove;
    int winner = status();
    
    // Terminal state: win, lose, or tie
    if (winner == O) {
        bestMove.score = 10;
        return bestMove;
    }
    if (winner == X) {
        bestMove.score = -10;
        return bestMove;
    }

    int emptySpots = 0;
    for (int i = 0; i < 9; i++) {
        if (grid[i] == 0) {
            emptySpots++;
        }
    }
    
    if (emptySpots == 0) {
        bestMove.score = 0;
        return bestMove;
    }
    
    // Store all possible moves 
    Moves *movesArray = malloc(emptySpots * sizeof(Moves));
    if (movesArray == NULL) {
        printf("%s", MSG[MEMORY_ERR]);
        exit(1);
    }

    int moveIndex = 0;
    for (int i = 0; i < 9; i++) {
        
        if (grid[i] == 0) {
            movesArray[moveIndex].index = i;
            grid[i] = player;

            if (player == O) {
                Moves result = miniMax(X);
                movesArray[moveIndex].score = result.score;
            } else {
                Moves result = miniMax(O);
                movesArray[moveIndex].score = result.score;
            }

            grid[i] = 0; // reset to original
            moveIndex++;
        }
        
    }

    bestMove.index = -1;
    if (player == O) {
        int bestScore = -10000;
        for (int i = 0; i < emptySpots; i++) {
            if (movesArray[i].score > bestScore) {
                bestScore = movesArray[i].score;
                bestMove.index = movesArray[i].index;
                bestMove.score = bestScore;
            }
        }
    } else {
        int bestScore = 10000;
        for (int i = 0; i < emptySpots; i++) {
            if (movesArray[i].score < bestScore) {
                bestScore = movesArray[i].score;
                bestMove.index = movesArray[i].index;
                bestMove.score = bestScore;
            }
        }
    }

    free(movesArray);
    return bestMove;
}

void playMax(int playerGridID){
    Moves positionToMove = miniMax(playerGridID);
    grid[positionToMove.index] = playerGridID; // AKA O = 1
}