/******************************************************************************
 *  Description     : Game mechanic functions for chain reaction game
 *  Author/s        : Gagasa, Emmanuel
 *                    Nono, Shenelle Andrea
 *                    Fornoles, Sabina
 *  Section         : S15 / S05
 ******************************************************************************/

#include <stdio.h>
#include "functions.h"

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
