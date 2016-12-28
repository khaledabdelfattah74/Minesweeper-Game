// Header Files
#ifndef header1
#define header1
#include <time.h>

// Constants :
#define NAMELEN 100     // Maximum Name Length
#define MAXGRIDSIZE 900 // Maximum Grid Size

// Data Structures :
// The Game Structure
typedef struct game
{
    char* disGrid;              // Displayed Grid
    char* grid;                 // The Opened Grid
    int width;                  // The Width of the grid
    int height;                 // The Height of the grid
    int gridSize;               // The Size of the Grid (= height * width)
    int numFlags;               // Number of Flags in the grid  (= 0)
    int numMoves;               // Number of Moves (= 0)
    int numQues;                // Number of Question marks in the grid (= 0)
    time_t startedTime;         // Stared time in seconds
} game;
// The player Structure
typedef struct player
{
    char name[NAMELEN];         // The player name
    int score;                  // The player Total score
    int highestScore;           // The player Highest score
} player;

// Functions :
// Centerize a line
void center(char* line);
// Print the welcome message
void welcomeMSG();
// Print the win message
void winMSG();
// Print the lose message
void loseMSG();
//exit message
void exitMSG();
// Scan a integer between min & max & Validate it
int scanInt (int min,int max);
// Scan a character & Validate it
char scanChar();
// Color The grid
void color(char x);
// Open the first cell & put mines & fill the grid & print it to a file
extern void initialization(game* curGame);
// Get the user action in the game & execute it (open & flag & question marks & unmark & save & exit) & return 0 for normal state & -1 for lose & -2 for save & -3 for exit & 1 for win
extern int execute(game* curGame);
// Take the user input
extern void *scanThread(void *arg);
// Get i & j for 2d array & return it's position in 1D array
int twoDArr(int i,int j,int height,int width);
// Print the grid with numbers and the user interface
void draw(game curGame);
// Draw The opened grid if file to debug
void drawInFile(game curGame);
// Return number of neighbor (X) in the Arr
int numOfXInArr(int height,int width,int row,int col,char x,char* arr);
// Open a cell and return state : 1 for win & 0 for normal & -1 for lose
extern int openCell(game* curGame,int row,int col,int* viewedGrid);
// Add the score and the name to the player's list
void win(int score);
// Sort leader board
extern void sortPlayerList(player* playerList,int numOfPlayers);
// To give some hints to the user
extern void hint(char* disGrid,int height,int width);

#endif
// Done

