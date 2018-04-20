// Header Files
#include "header1.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>

//Global variables
char order;                 // to take the user order in the game (for the thread function)
char wrongchoice[1000];     // To take user wrong input

// Functions :
// Centerize a line
void center(char* line)
{
    // Calculate the width of the terminal
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int widthOfScreen = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    // Print the line centered
    printf("%*s",(strlen(line)+widthOfScreen)/2,line);
}

// Print the welcome message
void welcomeMSG()
{
    center("              _                                                           \n");
    center("             | |                                                          \n");
    center("             | |                                                          \n");
    center("__      _____| | ___ ___  _ __ ___   ___                                  \n");
    center("\\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\                                 \n");
    center(" \\ V  V /  __/ | (_| (_) | | | | | |  __/                                 \n");
    center("  \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___|                                 \n");
    center(" _                    _                                                   \n");
    center("| |                  (_)                                                  \n");
    center("| |_ ___    _ __ ___  _ _ __   ___  _____      _____  ___ _ __   ___ _ __ \n");
    center("| __/ _ \\  | '_ ` _ \\| | '_ \\ / _ \\/ __\\ \\ /\\ / / _ \\/ _ \\ '_ \\ / _ \\ '__|\n");
    center("| || (_) | | | | | | | | | | |  __/\\__ \\\\ V  V /  __/  __/ |_) |  __/ |   \n");
    center(" \\__\\___/  |_| |_| |_|_|_| |_|\\___||___/ \\_/\\_/ \\___|\\___| .__/ \\___|_|   \n");
    center("                                                         | |              \n");
    center("                                                         |_|              \n");

}

// Print the win message
void winMSG ()
{
    printf("\n\n");
    center("                              .__        \n");
    center(" ___.__. ____  __ __  __  _  _|__| ____  \n");
    center("<   |  |/  _ \\|  |  \\ \\ \\/ \\/ /  |/    \\ \n");
    center(" \\___  (  <_> )  |  /  \\     /|  |   |  \\\n");
    center(" / ____|\\____/|____/    \\/\\_/ |__|___|  /\n");
    center(" \\/                                   \\/ \n");
}

// Print the lose message
void loseMSG()
{
    center("\n\n");
    center(">=>      >=>                           >=>           >===>        >=>>=>   >=======> \n");
    center(" >=>    >=>                            >=>         >=>    >=>   >=>    >=> >=>       \n");
    center("  >=> >=>       >=>     >=>  >=>       >=>       >=>        >=>  >=>       >=>       \n");
    center("    >=>       >=>  >=>  >=>  >=>       >=>       >=>        >=>    >=>     >=====>   \n");
    center("    >=>      >=>    >=> >=>  >=>       >=>       >=>        >=>       >=>  >=>       \n");
    center("    >=>       >=>  >=>  >=>  >=>       >=>         >=>     >=>  >=>    >=> >=>       \n");
    center("    >=>         >=>       >==>=>       >=======>     >===>        >=>>=>   >=======> \n");

}
void exitMSG(){
    center("\n\n");
    center("                         ||`    '||                     \n");
    center("                         ||      ||                     \n");
    center(".|''|, .|''|, .|''|, .|''||      ||''|, '||  ||` .|''|, \n");
    center("||  || ||  || ||  || ||  ||      ||  ||  `|..||  ||..|| \n");
    center("`|..|| `|..|' `|..|' `|..||.    .||..|'      ||  `|...  \n");
    center("    ||                                    ,  |'         \n");
    center(" `..|'                                     ''           \n");

}

// Scan a integer between min & max & Validate it
int scanInt (int min,int max)
{
    // Get the integer
    int x;
    char z[1000];
    gets(z);

    // Validate it
    if (strlen(z)<=2 && atoi(z) != 0 && (x=atoi(z)) <= max && x >= min)
    {
        if ((strlen(z)==1 && isdigit(z[0])) || (strlen(z)==2 && isdigit(z[0]) && isdigit(z[1])))
            return x;
    }
    else    // If it isn't valid take it again
    {
        center("Please Enter A Valid Number :  ");
        return scanInt(min,max);
    }
}

// Scan a character & Validate it
char scanChar ()
{
    // Get the character
    char in;
    char z[1000];
    gets(z);

    // If it isn't valid take it again
    while (strlen(z) != 1)
    {
        center("Please Enter A Valid Choice :  ");
        gets(z);
    }
    in = z[0];
    return in;
}

// Color The grid
void color (char x)
{
    // Color each character by different color
    int count = x - '0';
    if (x == 'X')
    {
        count = 9;
    }
    else if (x == 'F')
    {
        count = 15;
    }
    else if (x == 'M')
    {
        count = 11;
    }
    else if (x == '!')
    {
        count = 12;
    }
    else if (x == '?')
    {
        count = 13;
    }
    else if (x == '1')
    {
        count = 14;
    }
    else if (x == '_')
    {
        count=10;
    }
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, count);
}

// Open the first cell & put mines & fill the grid & print it to a file
void initialization(game* curGame)
{
    int i,j;
    // Get the first move
    center("Open The First Cell\n");
    center("Enter The Row    :  ");
    int row,col;
    row = scanInt(1,curGame->height+1);
    center("Enter The Column :  ");
    col = scanInt(1,curGame->width+1);
    row--;
    col--;
    curGame->startedTime=time(0);
    // Put the default value in the grid
    for(i = 0; i < curGame->height; i++)
    {
        for(j =0; j < curGame->width; j++)
        {
            curGame->grid[twoDArr(i,j,curGame->height,curGame->width)] = ' ';
        }
    }

    // Distribute the mines Randomly
    int pos; // position of mines
    int minesNum = 1 + (curGame->height * curGame->width) / 10;
    for(i = 0; i < minesNum; i++)
    {
        do
        {
            srand(rand()*rand()*time(0));
            pos = rand()%(curGame->height*curGame->width);
        }
        while(pos == twoDArr(row,col,curGame->height,curGame->width) || curGame->grid[pos] == '*' );
        curGame->grid[pos] = '*';
    }

    // Put numbers in cell
    int minesNumInCell;
    for (i = 0; i < curGame->height; i++)
    {
        for (j = 0; j < curGame->width; j++)
        {
            if (curGame->grid[twoDArr(i,j,curGame->height,curGame->width)] != '*')
            {
                minesNumInCell = numOfXInArr(curGame->height,curGame->width,i,j,'*',curGame->grid);
                if (minesNumInCell != 0)
                    curGame->grid[twoDArr(i,j,curGame->height,curGame->width)] = minesNumInCell + '0';
                else
                    curGame->grid[twoDArr(i,j,curGame->height,curGame->width)] =' ';
            }
        }
    }

    // Draw the grid
    drawInFile(*curGame);

    // Open the cell using the DFS search
    int viewedGrid[curGame->height * curGame->width];
    for(i = 0; i < curGame->height * curGame->width; i++)
    {
        viewedGrid[i] = 0;
    }
    openCell(curGame,row,col,viewedGrid);
}

// Get the user action in the game & execute it (open & flag & question marks & unmark & save & exit) & return 0 for normal state & -1 for lose & -2 for save & -3 for exit & 1 for win
int execute(game* curGame)
{
    // Take the user input with a thread
    int row=0,col=0;
    center("O For Opening The Cell Or Opening An Opened Cell, F For Putting Flags, Q For Putting ?, U For Unmark,S For Save,E for Exit From The Game,H For A Hint\n");
    center("Enter Your Choice :  ");
    order = 0;
    pthread_t pth;
    pthread_create(&pth,NULL,scanThread,NULL);
    // If the user didn't give any input for a minute redraw the grid
    time_t startTime = time(0);
    while(time(0) - startTime < 60)
    {
        usleep(1000);
        if(order != 0)
        {
            break;
        }
    }
    pthread_cancel(&pth);

    // If the user entered a wrong choice
    while (order != 'O' && order != 'F' && order != 'Q' && order != 'U' && order != 'S' && order != 'E' && order != 'H' && order != 0)
    {
        system("cls");
        draw(*curGame);
        center("Please Enter A Character Between (O,F,Q,U,S,H,E):  ");
        order = 0;
        pthread_create(&pth,NULL,scanThread,NULL);
        startTime = time(0);
        while(time(0) - startTime < 60)
        {
            usleep(1000);
            if(order != 0)
            {
                break;
            }
        }
        pthread_cancel(&pth);
    }

    // Check the order
    if (order=='O')
    {
        /*-------------------------------------------------Open------------------------------------------------------------------------*/
        // Take the position of the cell
        center("Enter The Cell Row's Number    :  ");
        row=scanInt(1,curGame->height+1);
        center("Enter The Cell Column's Number :  ");
        col = scanInt(1,curGame->width+1);
        row--;
        col--;
        curGame->numMoves++;

        // Make an array to use DFS and put it's value = 0
        int viewedGrid[curGame->height * curGame->width];
        int i;
        for(i = 0; i < curGame->height * curGame->width; i++)
        {
            viewedGrid[i] = 0;
        }
        // Check the position
        if (curGame->disGrid[twoDArr(row,col,curGame->height,curGame->width)] == 'X') // Open the cell if it's closed
        {
            return openCell(curGame,row,col,viewedGrid);
        }
        else if (curGame->disGrid[twoDArr(row,col,curGame->height,curGame->width)] >= '1' && curGame->disGrid[twoDArr(row,col,curGame->height,curGame->width)] <= '8' ) // if the cell contain a number  open all the reachable cells from it
        {
            // Count the flags
            int numOfFlagsInCell = numOfXInArr(curGame->height,curGame->width,row,col,'F',curGame->disGrid);
            if (numOfFlagsInCell == curGame->disGrid[twoDArr(row,col,curGame->height,curGame->width)]-'0') // Number of flags = the number
            {
                int k,l,state;
                for (k= row-1; k<=row+1; k++)
                {
                    for (l=col-1; l<=col+1; l++)
                    {
                        if (k < 0 || k >= curGame->height || l < 0 || l >= curGame->width)
                        {
                            continue;
                        }
                        if (curGame->disGrid[twoDArr(k,l,curGame->height,curGame->width)] != 'F')
                        {
                            //open neighbor cells till lose or win
                            state = openCell(curGame,k,l,viewedGrid);
                            if(state != 0)
                            {
                                return  state;
                            }
                        }
                    }
                }
            }
            else  // The number of flags != the number in the cell
            {
                printf("\n");
                center("Wrong This Cell Doesn't Have Enough Flags !! Choose Another Cell\n");
                center("Press Enter To Continue...  ");
                gets(wrongchoice);
            }
        }
        else // Wrong choice
        {
            printf("\n");
            center("Wrong You Opened An Opened Cell Choose Another Cell\n");
            center("Press Enter To Continue...  ");
            gets(wrongchoice);

        }

    }
    else if (order=='F')
    {
        /*-------------------------------------------------Flag------------------------------------------------------------------------*/
        // Take the position of the cell
        center("Enter The Cell Row's Number     :  ");
        row=scanInt(1,curGame->height+1);
        center("Enter The Cell Columns's Number :  ");
        col = scanInt(1,curGame->width+1);
        row--;
        col--;
        curGame->numMoves++;

        // Check the position
        if (curGame->disGrid[twoDArr(row,col,curGame->height,curGame->width)]!='X' && curGame->disGrid[twoDArr(row,col,curGame->height,curGame->width)]!='?') // We can't put a flag here
        {
            center("Wrong Choose Another Cell\n");
            center("Press Enter To Continue...  ");
            gets(wrongchoice);
        }
        else // We can put a flag here
        {
            curGame->disGrid[twoDArr(row,col,curGame->height,curGame->width)]='F';
            curGame->numFlags++;
        }
    }
    else if (order=='Q')
    {
        /*-------------------------------------------------Question---------------------------------------------------------------------*/
        // Take the position of the cell
        center("Enter The Cell Row's Number    :  ");
        row = scanInt(1,curGame->height+1);
        center("Enter The Cell Column's Number :  ");
        col = scanInt(1,curGame->width+1);
        row--;
        col--;
        curGame->numMoves++;

        // Check the position
        if (curGame->disGrid[twoDArr(row,col,curGame->height,curGame->width)]!='X') // We can't put a question mark here
        {

            center("Wrong Choose Another cell\n");
            center("Press Enter To Continue...  ");
            gets(wrongchoice);
        }
        else // We can put a question mark here
        {
            curGame->disGrid[twoDArr(row,col,curGame->height,curGame->width)]='?';
            curGame->numQues++;
        }
    }
    else if (order == 'U')
    {
        /*-------------------------------------------------Unmark------------------------------------------------------------------------*/
        // Take the position of the cell
        center("Enter The Cell Row's Number    :  ");
        row = scanInt(1,curGame->height+1);
        center("Enter The Cell Column's Number :  ");
        col = scanInt(1,curGame->width+1);
        row--;
        col--;
        curGame->numMoves++;

        // Check the position
        if (curGame->disGrid[twoDArr(row,col,curGame->height,curGame->width)]=='F') // There is a flag here remove it
        {
            curGame->numFlags--;
            curGame->disGrid[twoDArr(row,col,curGame->height,curGame->width)]='X';
        }
        else if (curGame->disGrid[twoDArr(row,col,curGame->height,curGame->width)]== '?') // There is a question mark here remove it
        {
            curGame->numQues--;
            curGame->disGrid[twoDArr(row,col,curGame->height,curGame->width)]='X';
        }
        else // Wrong choice
        {
            center("Wrong Choose Another cell\n");
            center("Press Enter To Continue...  ");
            gets(wrongchoice);
        }
    }
    else if (order == 'E')
    {
        /*-------------------------------------------------Exit------------------------------------------------------------------------*/
        // Make sure that the user want to exit
        printf("\n\n");
        center("Do You Want To Exit Without Saving ?\n");
        center("Press <Y> For Yes Or Press Enter To Resume The Game:  ");
        char p;
        p = scanChar();
        if (p == 'Y')
        {
            return -3;
        }
        else
        {
            return 0;
        }
    }
    else if (order == 'S')
    {
        /*-------------------------------------------------save------------------------------------------------------------------------*/
        // Get the user name
        char playerName[NAMELEN+1];
        center("Enter Your Name : ");
        gets(playerName);
        while (strlen(playerName)<1)
        {
            center("Please Enter Your Name Correctly\n");
            gets(playerName);
        }

        // Convert the name to lower case
        int i;
        for (i=0; i<strlen(playerName); i++)
        {
            playerName[i] = tolower(playerName[i]);
        }

        // Save the game in the file
        FILE* saveFile = fopen(playerName,"w");
        fprintf(saveFile,"%d\nS",curGame->gridSize);
        for(i = 0; i < curGame->gridSize; i++)
        {
            fprintf(saveFile,"%c%c",curGame->disGrid[i],curGame->grid[i]);
        }
        fprintf(saveFile,"\n");
        time_t timeNow = time(0);   // get time now
        int timeInSec = (timeNow - curGame->startedTime);
        fprintf(saveFile,"%d\n%d\n%d\n%d\n%d\n%d\n",curGame->height,curGame->width,curGame->numFlags,curGame->numMoves,curGame->numQues,timeInSec);
        fclose(saveFile);
        return -2;
    }
    /*-------------------------------------------------Hint------------------------------------------------------------------------*/
    else if (order=='H')
    {
        // Give the user a hint
        printf("\n\n");
        hint(curGame->disGrid,curGame->height,curGame->width);
        center("Press Enter To Continue...  ");
            gets(wrongchoice);

    }
    return 0;
}

// Take the user input
void *scanThread(void *arg)
{
    order = scanChar();
}

// Get i & j for 2d array & return it's position in 1D array
int twoDArr(int i,int j,int height,int width)
{
    return i + j * height;
}

// Print the grid with numbers and the user interface
void draw(game curGame)
{
    // Variables to use in loops
    int i,j;

    // Get the terminal width to calculate number of spaces used to center the grid
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int widthOfScreen = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int spacetocenter = (widthOfScreen-(6+curGame.width*4))/2;

    // Print the title line
    printf("%*s",spacetocenter,"");
    printf("     ");
    for(i =  0; i < curGame.width; i++)
    {
        if (i<=8) // To solve two-digit number's problem
        {
            printf("  %d ",i+1);
        }
        else
        {
            printf(" %d ",i+1);
        }
    }
    printf("\n\n");

    //Print the first break line
    printf("%*s",spacetocenter,"");
    printf("     ");
    printf("+");
    for(i = 0; i < curGame.width; i++)
    {
        printf("---+");
    }
    printf("\n");

    // Print the grid body
    for(i = 0; i < curGame.height; i++)
    {
        printf("%*s",spacetocenter,"");
        if (i<=8) // To solve two-digit number's problem
        {
            printf("%d    |",i+1);
        }
        else if (i<=98)
        {
            printf("%d   |",i+1);
        }
        for(j = 0; j < curGame.width; j++)
        {
            // Color each cell depend on it's value
            HANDLE hConsole;
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            color(curGame.disGrid[twoDArr(i,j,curGame.height,curGame.width)]);
            printf(" %c ",curGame.disGrid[twoDArr(i,j,curGame.height,curGame.width)]);
            SetConsoleTextAttribute(hConsole, 7);
            printf("|");
        }
        printf("\n");

        //Print the break line
        printf("%*s",spacetocenter,"");
        printf("     +");
        for(j = 0; j < curGame.width; j++)
        {
            printf("---+");
        }
        printf("\n");
    }

    // Print the game info
    char printstr[100];
    sprintf(printstr,"Number of Moves                            : %d\n",curGame.numMoves);
    center(printstr);
    sprintf(printstr,"Number of Cells Marked with Flags          : %d\n",curGame.numFlags);
    center(printstr);
    sprintf(printstr,"Number of Cells Marked with Question Marks : %d\n",curGame.numQues);
    center(printstr);

    // print time passed
    time_t timeNow = time(0);   // get time now
    int timeInSec = (timeNow - curGame.startedTime) % 60;
    int timeInMin = (timeNow - curGame.startedTime) / 60;
    if (curGame.numMoves == 0)
    {
        timeInSec=0;
        timeInMin=0;
    }
    sprintf(printstr,"Time passed :    %d minutes %d seconds\n\n",timeInMin,timeInSec);
    center(printstr);
}

// Draw The opened grid if file to debug
void drawInFile(game curGame)
{
    // Variables to use in loops
    int i,j;

    // Open the file
    FILE * fGrid;
    fGrid = fopen("grid.txt", "w");

    // Print the title line
    fprintf(fGrid, "    ");
    for(i = 0; i < curGame.width; i++)
    {
        if (i<=8)   // To solve two-digit number's problem
        {
            fprintf(fGrid, "  %d ",i+1);
        }
        else
        {
            fprintf(fGrid, " %d ",i+1);
        }
    }

    //Print the first break line
    fprintf(fGrid,"\n    +");
    for(i = 0; i < curGame.width; i++)
        fprintf(fGrid,"---+");
    fprintf(fGrid,"\n");

    // Print the grid body
    for(i = 0; i < curGame.height; i++)
    {
        if (i<=8)
        {
            fprintf(fGrid, "%d   |",i+1);	// print horizontal cells' numbers for one decimal number
        }
        else
        {
            fprintf(fGrid,"%d  |",i+1);	// print horizontal cells' numbers
        }
        for(j = 0; j < curGame.width; j++)
        {
            fprintf(fGrid, " %c |",curGame.grid[twoDArr(i,j,curGame.height,curGame.width)]); // FOR print grid
        }

        fprintf(fGrid, "\n");

        //Print the break line
        fprintf(fGrid, "    +");
        for(j = 0; j < curGame.width; j++)
            fprintf(fGrid, "---+");
        fprintf(fGrid, "\n");
    }
    fclose(fGrid);
}

// Return number of neighbor (X) in the Arr
int numOfXInArr(int height,int width,int row,int col,char x,char* arr)
{
    int i,j,sum = 0;
    for (i = row-1; i <= row+1; i++)
    {
        for (j = col-1; j <= col+1; j++)
        {
            if (i < 0 || i >= height || j < 0 || j >= width)
            {
                continue;
            }
            if (arr[twoDArr(i,j,height,width)] == x)
            {
                sum++;
            }
        }
    }
    return sum;
}

// Open a cell and return state : 1 for win & 0 for normal & -1 for lose
int openCell(game* curGame,int row,int col,int* viewedGrid)
{
    // Used in loops
    int i,j;

    // Check for lose
    if(curGame->grid[twoDArr(row,col,curGame->height,curGame->width)] == '*')
    {
        // view unopened mines
        for (i=0; i<curGame->height; i++)
        {
            for (j=0; j<curGame->width; j++)
            {

                if (curGame->grid[twoDArr(i,j,curGame->height,curGame->width)]=='*' && curGame->disGrid[twoDArr(i,j,curGame->height,curGame->width)] != 'F')
                {
                    curGame->disGrid[twoDArr(i,j,curGame->height,curGame->width)] = 'M';
                }
                if (curGame->disGrid[twoDArr(i,j,curGame->height,curGame->width)] == 'F'  && curGame->grid[twoDArr(i,j,curGame->height,curGame->width)] != '*')
                {
                    curGame->disGrid[twoDArr(i,j,curGame->height,curGame->width)] = '_';
                }
            }
        }
        curGame->disGrid[twoDArr(row,col,curGame->height,curGame->width)] = '!';
        return -1;
    }

    // Put 1 in the DFS array (I visited the cell)
    viewedGrid[twoDArr(row,col,curGame->height,curGame->width)] = 1;

    // Open neighbor cells if the cell is empty
    if(curGame->grid[twoDArr(row,col,curGame->height,curGame->width)] == ' ')
    {
        for (i = row-1; i <= row+1; i++)
        {
            for (j = col-1; j <= col+1; j++)
            {
                if (i < 0|| i >= curGame->height || j < 0 || j >= curGame->width)
                {
                    continue;
                }
                if(viewedGrid[twoDArr(i,j,curGame->height,curGame->width)] == 0 && curGame->disGrid[twoDArr(i,j,curGame->height,curGame->width)]!='F')
                {
                    openCell(curGame,i,j,viewedGrid);
                }
            }
        }
    }

    //Display the cell content
    curGame->disGrid[twoDArr(row,col,curGame->height,curGame->width)] = curGame->grid[twoDArr(row,col,curGame->height,curGame->width)];

    //Check for win
    int unOpenedCells = 0;
    for (i=0; i<curGame->height * curGame->width; i++)
    {
        if(curGame->disGrid[i] == '?' || curGame->disGrid[i] == 'X' || curGame->disGrid[i] == 'F')
        {
            unOpenedCells++;
        }
    }
    if(unOpenedCells == 1 + (curGame->height * curGame->width) / 10)
    {
        curGame->disGrid[twoDArr(row,col,curGame->height,curGame->width)] = curGame->grid[twoDArr(row,col,curGame->height,curGame->width)];
        return 1;
    }
    return 0;
}

// Add the score and the name to the player's list
void win(int score)
{
    // Take the user name
    char name[NAMELEN];
    center("Enter Your Name :  ");
    gets(name);
    while (strlen(name) < 1)
    {
        center("Please Enter Your Name Correctly\n");
        gets(name);
    }

    // Convert the name to lower case
    int i;
    for (i=0; i<strlen(name); i++)
    {
        name[i]=(tolower(name[i]));
    }

    // Open the player's list file
    FILE* playerFileRead = fopen("playerList","r");
    int numOfPlayers;
    int flag; //Used to check if the player exists in the player list
    player* playerList;

    // Check  if the file exists
    if(playerFileRead == 0) // This's the first player to win
    {
        // Add the player to the player's list structure
        numOfPlayers = 1;
        playerList = malloc(sizeof(player));
        playerList->highestScore = playerList->score = score;
        strcpy(playerList->name,name);
    }
    else // This isn't the first player to win
    {
        // Add the players to the player's list structure array
        fscanf(playerFileRead,"%d\n",&numOfPlayers);
        playerList = malloc(sizeof(player) * (numOfPlayers+1));
        for(i = 0; i < numOfPlayers; i++)
        {
            fgets(playerList[i].name,NAMELEN,playerFileRead);
            fscanf(playerFileRead,"%d %d\n",&playerList[i].score,&playerList[i].highestScore);
            playerList[i].name[strlen(playerList[i].name) - 1]= '\0';
            // If the player exist in the player's list
            if(strcmp(playerList[i].name,name) == 0)
            {
                playerList[i].score+=score;
                if(score > playerList[i].highestScore) // The player break his record
                {
                    playerList[i].highestScore = score;
                }
                flag = 0;
            }
        }
        if(flag) //The player doesn't exist in the player's list
        {
            // Add the players to the player's list structure array
            playerList[numOfPlayers].highestScore = playerList[numOfPlayers].score = score;
            strcpy(playerList[numOfPlayers].name,name);
            numOfPlayers++;
        }

    }
    fclose(playerFileRead);

    // Start writing the player's list file from the structure array
    FILE* playerFileWrite = fopen("playerList","w");
    fprintf(playerFileWrite,"%d\n",numOfPlayers);
    for(i = 0; i < numOfPlayers; i++)
    {
        fprintf(playerFileWrite,"%s\n",playerList[i].name);
        fprintf(playerFileWrite,"%d %d\n",playerList[i].score,playerList[i].highestScore);
    }
    fclose(playerFileWrite);
    free(playerList);
}

// Sort leader board
void sortPlayerList(player* playerList,int numOfPlayers)
{
    int i,j,maxi;
    player tmp;
    for(i = 0; i < numOfPlayers; i++)
    {
        maxi = i;
        for(j = i+1; j < numOfPlayers; j++)
        {
            if(playerList[j].score > playerList[maxi].score)
            {
                maxi = j;
            }
        }
        tmp = playerList[i];
        playerList[i] = playerList[maxi];
        playerList[maxi] = tmp;
    }
}

// To give some hints to the user
void hint(char* disGrid,int height,int width)
{
    int i,j,k,l; // Used for loops
    char msg[100]; // Used to center the hint
    float probOfMine;
    int numOfXArountCell;
    for(i = 0; i < height; i++)
    {
        for(j = 0; j < width; j++)
        {
            if(disGrid[twoDArr(i,j,height,width)] >= '1' && disGrid[twoDArr(i,j,height,width)] <= '8')
            {
                // Calculate the probability of mine around a numbered cell
                numOfXArountCell = numOfXInArr(height,width,i,j,'X',disGrid) + numOfXInArr(height,width,i,j,'?',disGrid);
                probOfMine = (float)(disGrid[twoDArr(i,j,height,width)]-'0' - numOfXInArr(height,width,i,j,'F',disGrid)) / numOfXArountCell;
                // If there isn't a mine without flag for sure
                if(probOfMine == 0 && numOfXArountCell != 0)
                {
                    sprintf(msg,"You Can Open All Cells (Without Flags) Around (%d,%d)\n",i+1,j+1);
                    center(msg);
                    return;
                }
                // If there's a mine in every closed cell around this cell
                else if(probOfMine == 1)
                {
                    for (k = i-1; k <= i+1; k++)
                    {
                        for (l = j-1; l <= j+1; l++)
                        {
                            if (k < 0 || k >= height || l < 0 || l >= width || (k == i && l == j))
                                continue;
                            if(disGrid[twoDArr(k,l,height,width)] != 'X' && disGrid[twoDArr(k,l,height,width)] != '?')
                                continue;
                            //If the cell is a mine for sure
                            sprintf(msg,"The cell (%d,%d) is a mine\n",k+1,l+1);
                            center(msg);
                            return;
                        }
                    }
                }
            }
        }
    }

    // If There isn't a confirmed hint
    sprintf(msg,"No Confirmed Hint!!\n");
    center(msg);
}
// Done

