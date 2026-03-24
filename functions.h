/* ============================= */
/*        STRUCTURES             */
/* ============================= */

typedef struct
{
    int good;
    int go;
    int start;
    int over;
    int found;
    int val;

    int R[4][4];
    int B[4][4];
    int S[4][4];
    int T[4][4];

} GameState;


/* ============================= */
/*     FUNCTION PROTOTYPES       */
/* ============================= */

/* Initialization */
void initializeGame(GameState *g)
{
    int i, j;

    g->good = 0;
    g->go = 0; 
    g->start = 1;
    g->over = 0;
    g->found = 0;
    g->val = 0;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            g->R[i][j] = 0;
            g->B[i][j] = 0;
            g->S[i][j] = 0;
            g->T[i][j] = 0;
        }
    }
}

/* Display */
void displayBoard(GameState g);

/* Input */
void getMove(int *row, int *col);

int isValidPos(int row, int col)
{
    int valid = 0;

    if (row>=0 && row<4 && col>=0 && col<4)
        valid = 1;

    return valid;
}

/* Game Actions */
void removePos(GameState *g, int row, int col)
{
    // code here but idk wtf to do
}

void replacePos(GameState *g, int row, int col);
void expandPos(GameState *g, int row, int col);
void updatePos(GameState *g, int row, int col);
void nextPlayerMove(GameState *g, int row, int col);

/* Utility Functions */
int countPieces(int board[4][4]);
int countFreeCells(GameState g);

/* Game Status */
void checkGameOver(GameState *g);

void showResult(GameState g)
{
    int nRed, nBlue; // pang count sa pieces

    nRed = countPieces(g.R); //array pointing to red pieces nd so on..
    nBlue = countPieces(g.B);

    printf("| ------- GAME OVER ------- |"); // idk pa ano display adjust k nalang soon
    printf("Red pieces: %d\n", nRed);
    printf("Blue pieces: %d\n", nBlue);

    if (nRed > nBlue)
        printf("WINNER: RED\n");
    else if (nBlue > nRed)
        printf("WINNER: BLUE\n");
    else
        print("RESULT: DRAW\n");
}