#include <stdio.h>
#include "functions.h"
/*
UPDATE LOG AS OF 24/03/2026:
- added InitializeGame() function
- added isValidPos() although idk kung tama na ba yun
- added showResult ()
*/

int main ()
{
    GameState game;
    int row, col;

    /* Initializing the game */
    initializeGame(&game);

    /* Loop continues as long as game.over is 0 */
    while (game.over == 0) {
        displayBoard(game);
        getMove(&game, &row, &col);
        
        nextPlayerMove(&game, row, col);
        checkGameOver(&game);
    }

    /* Displays final board state and shows winner*/
    displayBoard(game);
    showResult(game);
    
    return 0;
}
