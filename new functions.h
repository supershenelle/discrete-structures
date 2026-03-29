#include <stdio.h>

/* ============================= */
/* STRUCTURES             */
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

// function prototypes
void displayDivider();
void initializeGame(GameState *g); 
void displayBoard(GameState g);
void getMove(GameState *g, int *row, int *col); 
int isValidPos(GameState g, int row, int col); 
void removePos(GameState *g, int row, int col); 
void replacePos(GameState *g, int row, int col); 
void expandPos(GameState *g, int row, int col); 
void updatePos(GameState *g, int row, int col); 
void nextPlayerMove(GameState *g, int row, int col);
int countPieces(int board[4][4]);
int countFreeCells(GameState g);
void checkGameOver(GameState *g);
void showResult(GameState g);

// function implementations
void displayDivider() 
{
    printf("===========================================\n");
}

void initializeGame(GameState *g)
{
    int i, j;

    g->good = 0;   
    g->go = 1;     
    g->start = 1;  
    g->over = 0;   
    g->found = 0;  
    g->val = 1;    

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

void displayBoard(GameState g)
{
    int i, j;
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
            if(g.R[i][j] == 1)
                printf(" %sR%s %s|%s", RED, RESET, BOARD, RESET);
            else if (g.B[i][j] == 1)
                printf(" %sB%s %s|%s", BLUE, RESET, BOARD, RESET);
            else
                printf("   %s|%s", BOARD, RESET); 
        }
        printf("\n%s  +---+---+---+%s\n", BOARD, RESET);
    }

    if (g.go == 1) 
        printf("Current Turn: %sRED%s\n", RED, RESET);
    else 
        printf("Current Turn: %sBLUE%s\n", BLUE, RESET);

    printf("Current Val: %d\n", g.val);
    displayDivider();
}

void getMove(GameState *g, int *row, int *col)
{
    int r, c;
    int validInput = 0;
    char dump;

    while (validInput == 0)
    {
        printf("--> ENTER YOUR MOVE (ROW AND COLUMN):\n");
        printf("ENTER ROW: ");
        if (scanf("%d", &r) == 1)
        {
            printf("ENTER COLUMN: ");
            if (scanf("%d", &c) == 1)
            {
                if (isValidPos(*g, r, c) == 0)
                {
                    printf("\n--> USER HAS INPUTTED AN INVALID POSITION.\n");
                    printf("--> INPUT MUST BE BETWEEN 1-3 ONLY. TRY AGAIN.\n");
                    displayDivider();
                }
                else if (g->start == 1 && g->S[r][c] == 1)
                {
                    printf("\n--> POSITION IS OCCUPIED. TRY AGAIN.\n");
                    displayDivider();
                }
                else if (g->start == 0 && !((g->go == 1 && g->R[r][c] == 1) || (g->go == 0 && g->B[r][c] == 1)))
                {
                    printf("\n--> CHOOSE YOUR OWN PIECE TO EXPAND. TRY AGAIN.\n");
                    displayDivider();
                }
                else
                {
                    *row = r;
                    *col = c;
                    validInput = 1;
                }
            }
            else 
            { 
                while (scanf("%c", &dump) == 1 && dump != '\n'); 
            }
        }
        else 
        { 
            while (scanf("%c", &dump) == 1 && dump != '\n'); 
        }
    }
}

int isValidPos(GameState g, int row, int col) 
{
    int valid = 0;
    if (row >= 1 && row <= 3 && col >= 1 && col <= 3)
    {
        valid = 1;
    }
    return valid;
}

void removePos(GameState *g, int row, int col) 
{
    if (g->go == 1) 
    {
        g->R[row][col] = 0; 
    }
    else            
    {
        g->B[row][col] = 0; 
    }
    g->S[row][col] = 0; 
}

void replacePos(GameState *g, int row, int col) 
{
    if (isValidPos(*g, row, col) == 1)
    {
        g->found = 0; 

        if (g->go == 1) 
        {
            if (g->B[row][col] == 1) 
            { 
                g->B[row][col] = 0; 
                g->found = 1; 
            }
            else if (g->R[row][col] == 1) 
            { 
                g->found = 1; 
            }
            
            if (g->R[row][col] == 0) 
            {
                g->R[row][col] = 1;
            }
        } 
        else 
        {
            if (g->R[row][col] == 1) 
            { 
                g->R[row][col] = 0; 
                g->found = 1; 
            }
            else if (g->B[row][col] == 1) 
            { 
                g->found = 1; 
            }

            if (g->B[row][col] == 0) 
            {
                g->B[row][col] = 1;
            }
        }

        if (g->found == 1 && g->S[row][col] == 0) 
        {
            g->S[row][col] = 1;
            g->found = 0;
        } 
        else if (g->found == 1 && g->S[row][col] == 1 && g->T[row][col] == 0) 
        {
            g->T[row][col] = 1; 
            expandPos(g, row, col);
        }
    }
}

void expandPos(GameState *g, int row, int col) 
{
    removePos(g, row, col); 
    replacePos(g, row - 1, col); 
    replacePos(g, row + 1, col); 
    replacePos(g, row, col - 1); 
    replacePos(g, row, col + 1); 
}

void updatePos(GameState *g, int row, int col) 
{
    g->good = 0; 
    if (g->S[row][col] == 0) 
    {
        g->S[row][col] = 1;
        g->good = 1; 
    } 
    else if (g->good == 0 && g->S[row][col] == 1 && g->T[row][col] == 0) 
    {
        g->T[row][col] = 1;
        expandPos(g, row, col);
        g->good = 1; 
    }
}

void nextPlayerMove(GameState *g, int row, int col) 
{
    int i, j;
    
    if (g->over == 0) 
    {
        if (g->start == 1) 
        {
            if (g->go == 1) 
            {
                g->R[row][col] = 1; 
            }
            else            
            {
                g->B[row][col] = 1; 
            }
            g->S[row][col] = 1;
            g->good = 1;
        } 
        else 
        {
            if ((g->go == 1 && g->R[row][col] == 1) || (g->go == 0 && g->B[row][col] == 1)) 
            {
                updatePos(g, row, col);
                g->good = 1;
            }
        }

        if (g->start == 1 && countPieces(g->R) >= 1 && countPieces(g->B) >= 1) 
        {
            g->start = 0;
        }

        if (g->good == 1) 
        {
            if (g->go == 1)
            {
                g->go = 0;
            }
            else
            {
                g->go = 1;
            }
            
            g->val = g->val + 1;         
            for (i = 0; i < 4; i++) 
            {
                for (j = 0; j < 4; j++) 
                {
                    g->T[i][j] = 0; 
                }
            }
            g->good = 0;
        }
    }
}

int countPieces(int board[4][4]) 
{
    int i, j, count = 0;
    for(i = 1; i <= 3; i++)
    {
        for(j = 1; j <= 3; j++)
        {
            if(board[i][j] != 0) 
            {
                count++;
            }
        }
    }
    return count;
}

int countFreeCells(GameState g) 
{
    int i, j, occupied = 0;
    for(i = 1; i <= 3; i++)
    {
        for(j = 1; j <= 3; j++)
        {
            if(g.S[i][j] == 1) 
            {
                occupied++;
            }
        }
    }
    return 9 - occupied;
}

void checkGameOver(GameState *g) 
{
    int rCount = countPieces(g->R);
    int bCount = countPieces(g->B);
    int free = countFreeCells(*g);

    if (free == 3 || g->val >= 20 || (g->start == 0 && (rCount == 0 || bCount == 0)))
    {
        g->over = 1;
    }
}

void showResult(GameState g) 
{
    int nRed = countPieces(g.R);
    int nBlue = countPieces(g.B);
    printf("| -------------- GAME OVER -------------- |\n");
    printf("%sRed%s pieces: %d\n", "\033[31m", "\033[0m", nRed);
    printf("%sBlue%s pieces: %d\n", "\033[34m", "\033[0m", nBlue);

    if (nRed > nBlue)      
    {
        printf("--> WINNER: %sRED%s\n", "\033[31m", "\033[0m");
    }
    else if (nBlue > nRed) 
    {
        printf("--> WINNER: %sBLUE%s\n", "\033[34m", "\033[0m");
    }
    else                   
    {
        printf("--> RESULT: %sDRAW%s\n", "\033[33m", "\033[0m");
    }
}
