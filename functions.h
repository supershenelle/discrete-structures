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
/* FUNCTION PROTOTYPES      */
/* ============================= */


void initializeGame(GameState *g); 
void displayBoard(GameState g);

void getMove(int *row, int *col); 
int isValidPos(int row, int col); 

void removePos(GameState *g, int row, int col); 
void replacePos(GameState *g, int row, int col); 
void expandPos(GameState *g, int row, int col); 
void updatePos(GameState *g, int row, int col); 
void nextPlayerMove(GameState *g, int row, int col);

int countPieces(int board[4][4]);
int countFreeCells(GameState g);

void checkGameOver(GameState *g);
void showResult(GameState g);

/* ------------------------------------------------------------- */
    
/* Initialization */
void initializeGame(GameState *g)
{
    int i, j;

    g->good = 0; // 0 if invalid move, 1 if valid move
    g->go = 0; // 0 if red, 1 if blue
    g->start = 1; // 0 if normal gameplay, 1 if starting
    g->over = 0; // 0 if game running, 1 if game ends
    g->found = 0; 
    g->val = 0; // general counting variable for values

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            g->R[i][j] = 0; //red
            g->B[i][j] = 0; //blue
            g->S[i][j] = 0; //state (if may naka-occupy)
            g->T[i][j] = 0; //temp if needed
        }
    }
}

/* Display */
void displayBoard(GameState g)
{
    /*
   displayboard has 3 sets  in a visual grid
   R - Red
   B - Blue
   S - Occupied

   4x4 matrix sya and provides feedback sa kung sino is currently moving and ung current "population" ng board
   accd sa specs, may red and blue pieces, and grids

   for red pieces, "\033[1;31m"
   for blue, "\033[1;34m"
   grid line color, "\033[0;37m"
   para maiwasan color bleed, ung reset: "\033[0m"
    */

    int i, j;

    // define the colors
    const char *RED = "\033[1;31m";  
    const char *BLUE = "\033[1;34m";
    const char *BOARD = "\033[0;37m";  
    const char *RESET = "\033[0m";

    printf("\n%s    1   2   3%s\n", BOARD, RESET);
    printf("%s  +---+---+---+%s\n", BOARD, RESET);

    for(i = 1; i <= 3; i++) 
    {

        printf("%s%d |%s", BOARD, i, RESET);

        for(j = 1; j <= 3; j++) 
        {
        // eto ung if nasa set R or B ung pieces
            if(g.R[i][j] == 1)
                printf(" %sR%s %s|%s", RED, RESET, BOARD, RESET);
            else if (g.B[i][j] == 1)
                printf(" %sB%s %s|%s", BLUE, RESET, BOARD, RESET);
            else
                printf("   %s|%s", BOARD, RESET); 
        }
        // print pang separate pero in board color
        printf("\n%s  +---+---+---+%s\n", BOARD, RESET);
    }

    // eto ung current turn
    if (g.go == 0) 
        printf("Current Turn: %sRED%s\n", RED, RESET);
    else 
        printf("Current Turn: %sBLUE%s\n", BLUE, RESET);

    // display population
    printf("Current Val: %d\n", g.val);
}

// input
void getMove(int *row, int *col)
{
    int r, c;
    int valid = 0;

    while (!valid)
    {
        printf("Enter your move (row and col, 1-3): ");
        
        // check if input is numeric
        if (scanf("%d %d", &r, &c) == 2)
        {
            // check if the position is within set M
            if (isValidPos(r, c))
            {
                *row = r;
                *col = c;
                valid = 1;
            }
            else
            {
                printf("Invalid position. Must be between 1 and 3.\n");
            }
        }
        else
        {
            printf("Invalid input format. Please enter two integers.\n");
            // getchar pampatanggal infinite loops
            while (getchar() != '\n');
        }
    }
}

int isValidPos(int row, int col)
{
    int valid = 0;

    if (row >= 1 && row <= 3 && col >= 1 && col <= 3)
        valid = 1;

    return valid;
}

/* Game Actions */
void removePos(GameState *g, int row, int col) 
{
    /* Only remove the piece at the specific coordinate */
    g->R[row][col] = 0;
    g->B[row][col] = 0;
    g->S[row][col] = 0;
    g->T[row][col] = 0;
}

void replacePos(GameState *g, int row, int col) 
{
    if (isValidPos(row, col)) 
    {
        int found = 0;
        /* Capture Logic */
        if (g->go == 0) 
        { // Red Turn
            if (g->B[row][col]) 
                g->B[row][col] = 0; found = 1;
            else if (g->R[row][col]) 
                found = 1;
            else
                g->R[row][col] = 1;
        } 
        else 
        { // Blue Turn
            if (g->R[row][col])
                g->R[row][col] = 0; found = 1;
            else if (g->B[row][col])
                found = 1;
            else 
                g->B[row][col] = 1;
        }

        /* Chain Reaction Trigger */
        if (found && g->S[row][col] == 0)
            g->S[row][col] = 1;
        else if (found && g->S[row][col] == 1 && g->T[row][col] == 0) 
        {
            g->T[row][col] = 1;
            expandPos(g, row, col); // RECURSIVE CALL
        }
    }
}

void expandPos(GameState *g, int row, int col) 
{
    removePos(g, row, col);
    /* Spread to neighbors - each will call replacePos again */
    replacePos(g, row - 1, col);
    replacePos(g, row + 1, col);
    replacePos(g, row, col - 1);
    replacePos(g, row, col + 1);
}

void updatePos(GameState *g, int row, int col) 
{
    if (g->S[row][col] == 0) 
    {
        g->S[row][col] = 1;
        g->good = 1;
    } 
    else if (g->T[row][col] == 0) 
    {
        g->T[row][col] = 1;
        expandPos(g, row, col);
        g->good = 1;
    }
}

void nextPlayerMove(GameState *g, int row, int col) 
{
    int i, j;
    if (g->over == 0) {
        if (g->start == 1) 
        {
            if (g->go == 0)
                g->R[row][col] = 1; 
            else 
                g->B[row][col] = 1;
            g->S[row][col] = 1; g->good = 1;
        } 
        else
            /* Validates that the player clicked their own piece */
            if ((g->go == 0 && g->R[row][col] == 1) || (g->go == 1 && g->B[row][col] == 1))
                updatePos(g, row, col);
        
        /* Switches turn and increments counter if the move was valid */
        if (g->good == 1) 
        {
            if (g->go == 0) 
                g->go = 1;
            else 
                g->go = 0;
            g->val = g->val + 1;
            g->good = 0;
            for (i = 1; i <= 3; i++) {
                for (j = 1; j <= 3; j++) { g->T[i][j] = 0; }
            }
        }
        /* Ends setup phase once both players have placed a piece */
        if (g->start == 1 && countPieces(g->R) >= 1 && countPieces(g->B) >= 1)
            g->start = 0;
    }
}

/* Utility Functions */
int countPieces(int board[4][4]) 
{
    int i, j, count = 0;
    for(i = 1; i <= 3; i++)
        for(j = 1; j <= 3; j++)
            if(board[i][j])
                count++;
    
    return count;
}

int countFreeCells(GameState g) 
{
    int i, j, occupied = 0;
    for(i = 1; i <= 3; i++)
        for(j = 1; j <= 3; j++)
            if(g.R[i][j] == 1|| g.B[i][j] == 1) 
                occupied++;
    return 9 - occupied; // 4x4 matrix
}

/* Game Status */
void checkGameOver(GameState *g) 
{
    int rCount = countPieces(g->R);
    int bCount = countPieces(g->B);
    int free = countFreeCells(*g);

    if (free == 3 || g->val >= 20 || (!g->start && (rCount == 0 || bCount == 0)))
        g->over = 1;
}

void showResult(GameState g)
{
    int nRed, nBlue; // pang count sa pieces

    nRed = countPieces(g.R); //array pointing to red pieces nd so on..
    nBlue = countPieces(g.B);

    printf("| ------- GAME OVER ------- |\n"); // idk pa ano display adjust k nalang soon
    printf("Red pieces: %d\n", nRed);
    printf("Blue pieces: %d\n", nBlue);

    if (nRed > nBlue)
        printf("WINNER: RED\n");
    else if (nBlue > nRed)
        printf("WINNER: BLUE\n");
    else
        printf("RESULT: DRAW\n");
}
