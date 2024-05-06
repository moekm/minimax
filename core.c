#include <stdio.h>
#include <stdbool.h>
/**
 * Simple Tic Tac Toe game using the MiniMax Algorithm 
 * TODO:
 * - Add row column number support A1 - B3 - C2
 * - Add an option to let players choose their symbols
 * - Add GUI Support
 * - Add Custom Game Modes (EASY - NORMAL - HARD [aka. minimax])
*/

void update(int grid[], char symbols[]);            // updates the grid
int status(int grid[], bool *stop);                 // checks for wins 
int selected(int grid[], int input);                // checks if the square is already selected
void plyturn(int num, int *player);                 // calcuates the player turns from the round

enum MSGID { SELECTED , NUMBERS_ONLY , WIN , DRAW , UNKOWN_ERR};

int main() {

    /**
     * Drawing the grid in the terminal (NO GUI SUPPORT YET)
     *     1   2   3
     * A | O | X | O |    
     * B | X | X | O |
     * C | X | O | X |
    */

   int  grid[9] = {0,0,0,0,0,0,0,0,0};
   int  input, round, winner;
   int player[2]; // {playerID, playerDisplayNumber}
   char symbols[2] = {'X', 'O'};
   char MSG[][100] = {
    "\nThe square has already been selected :(",
    "\nvalid inputs are between 1-9",
    "\nYay! %c HAS WON!",
    "\nDRAW! Guess you're both stuck in the \"Suck Zone\" >:)",
    "\nUnable to load the game :O" // no display yet
   };


    // initial -> grid passed as a pointer
    update(grid, symbols); 

    /*
    * Mode 1: Local/Offline
    */

    while (round < 9) {

        bool redo = true; 
        bool stop = false;
       
        do {
            
            plyturn(round, player); // check whose turn it is
            printf("\nPlayer (%d):", player[1]);

            if (scanf("%d", &input) != 1){
            getchar(); // consume '\n'
            printf(MSG[NUMBERS_ONLY]);
            continue;
            }

            // check for allowed numbers 1-9
            if (input > 9 || input <= 0){
                printf(MSG[NUMBERS_ONLY]);
                continue;
            } 

            if (selected(grid, input)){
                redo = true;
                update(grid, symbols);
                printf("\n%s", MSG[SELECTED]);
            } else {
                redo = false;
                grid[input - 1] = player[0]; 
                update(grid, symbols);
            }

        } while (redo);

        winner = status(grid, &stop);
        if (stop) break; 

        // printf("\nround: %d", round);
        round++;
    }
    
    if (winner == -1) printf(MSG[WIN], symbols[0]);
    if (winner == 1) printf(MSG[WIN], symbols[1]);
    if (winner == 0) printf(MSG[DRAW]);

    return 0;
}

void update(int grid[], char symbols[]){

    // clears previous logs
    printf("\033[H\033[J");

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

int selected(int grid[], int input){
    if (grid[input - 1] == -1 || grid[input - 1] == 1){
        return 1; // true
    }
}

int status(int grid[], bool *stop){

    int player;

    for (int i = 0; i < 3; i++){

        // checks for column wins
        if (grid[i] == grid[i + 3] && grid[i] == grid[i + 6] && grid[i] != 0){
            player = grid[i];
            *stop = true;
        } 
        
        // checks for row wins
        if (grid[i * 3] == grid[(i * 3) + 1] && grid[i * 3] == grid[(i * 3) + 2] && grid[i * 3] != 0){
            player = grid[i * 3];
            *stop = true;
        } 
    }

    // check for diagonals
    if (grid[0] == grid[4] && grid[0] == grid[8] && grid[0] != 0){
        player = grid[0];
        *stop = true;
    }

     if (grid[2] == grid[4] && grid[2] == grid[6] && grid[2] != 0){
        player = grid[2];
        *stop = true;
    }

    if (*stop) return player;
}

void plyturn(int num, int *player){

    num = num % 2;
    if (num == 1){
        *(player) = 1; // O
        *(player + 1) = 2; 
    } 
    
      if (num == 0){
        *(player) = -1; // X
        *(player + 1) = 1; 
    } 
}