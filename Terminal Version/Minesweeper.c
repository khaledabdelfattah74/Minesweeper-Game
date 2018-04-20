// Header Files
#include "Minesweeper.h"
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
char wrongchoice[1000]; // To take user wrong input

void mineSweeperGame()
{
    // Main variables
    int i;                      // Used in loops
    char printstr[100];         // Used when we centerize printf-style string
    char in = 0;                // Used to take user choice in the main menu
    game curGame;               // Declare the game structure
    char disGrid[MAXGRIDSIZE];  // Declare the game structure
    char grid[MAXGRIDSIZE];     // Declare the game structure
    int loaded = 0;             // Used to determine if the game is loaded or not

    // Print the welcome messages
    printf("\n\n\n\n\n\n\t\t");
    welcomeMSG();
    center("Press Enter To Continue  ");
    gets(wrongchoice);
    system("cls");
    system("COLOR 0F");
    printf("\n\n\n");
    center("Welcome to Minesweeper!!\n");

    // Start the game loop
    while (in != 'E')
    {
        if(!loaded)
        {
            system("cls");
            // Print the main menu
            center("To start new game          press (N)\n");
            center("To load game               press (L)\n");
            center("To read game instruction   press (R)\n");
            center("To see player's list       press (P)\n");
            center("To exit                    press (E)\n");
            center("Enter Your Choice :  ");
            in = scanChar(in);
            while (in != 'N' && in != 'L' && in != 'P' &&  in != 'R' && in != 'E')
            {

                center("Please Enter A Character Between (N,L,P,R,E)\n");
                center("Enter Your Choice :  ");
                in = scanChar(in);
            }
            system("cls");
        }

        // Check for user choice
        if (in == 'N' || loaded) // new game or the game is loaded
        {
            int sum1 = 0; // Used to check wining from the first move
            if(!loaded)
            {
                /*-------------------------------------------------New Game---------------------------------------------------------------------*/
                //Get the height and width
                int height,width;
                center("Enter The Grid Height :  ");
                height = scanInt(2,30);
                center("Enter The Grid Width  :  ");
                width  = scanInt(2,30);
                system("cls");
                //Make the game structure
                curGame.height = height;
                curGame.width = width;
                curGame.gridSize = height * width;
                curGame.numFlags = 0;
                curGame.numMoves = 0;
                curGame.numQues = 0;
                curGame.disGrid = disGrid;
                curGame.grid = grid;
                int i,j;
                for(i = 0; i < curGame.gridSize; i++)
                {
                    disGrid[i] = 'X';
                }

                //draw the grid & start the time
                draw(curGame);

                //Initialize the grid & Get the first move
                initialization(&curGame);

                // Check for win from t
                for (i=0; i<height; i++)
                {
                    for (j=0; j<width; j++)
                    {
                        if (disGrid[twoDArr(i,j,height,width)] == 'X')
                        {
                            sum1++;
                        }
                    }
                }
                int numOfMines = 1 +(height*width)/10;
                if (sum1==numOfMines)
                {
                    sum1=-1;
                }
                curGame.numMoves++;
            }
            int state = 0;              // 1 for win , 0 for normal state , -1 for lose, -2 for save
            if (sum1 == -1)
            {
                state = 1;
            }

            // Play the game until lose or win
            while (state == 0)
            {
                system("cls");
                draw(curGame);
                state = execute(&curGame);
            }
            system("cls");
            // Check the state
            if(state == 1)
            {
                /*-------------------------------------------------Win------------------------------------------------------------------------*/
                // Display the win messages
                draw(curGame);
                winMSG();
                printf("\n\n");
                center("Win\n\n");

                // Calculate the score & print it
                time_t timeNow = time(0);   // get time now
                int timeInSec = (timeNow - curGame.startedTime) % 60;
                int timeInMin = (timeNow - curGame.startedTime) / 60;
                if (timeInMin+timeInSec==0){
                    timeInSec+=1;
                }
                float score = (pow(curGame.height,4)*pow(curGame.width,4))/(((timeInMin*60)+(timeInSec%60))*curGame.numMoves);
                sprintf(printstr,"Your Score Is %d\n\n",(int)score);
                center(printstr);
                center("Congratulations\n\n");

                // Take his name to add him to the player's list
                win(score);
                loaded=0;
            }
            else if (state == -2)
            {
                /*-------------------------------------------------Save------------------------------------------------------------------------*/
                printf("\n\n");
                center("File Saved\n\n");
                char ch;
                center("If You Want to Resume The Game Press <Y> Otherwise Press Enter\n");
                center("Enter Your Choice :  ");
                ch = scanChar(ch);
                if (ch == 'Y')
                {
                    loaded = 1;
                }
                else
                {
                    loaded = 0;
                }
            }
            else if (state == -1)
            {
                /*-------------------------------------------------lose------------------------------------------------------------------------*/
                // Print the lose messages & the grid
                loseMSG();
                printf("\n\n\n");
                center("Game Over\n\n\n");
                draw(curGame);
                center("Press Enter To Continue...  ");
                gets(wrongchoice);
                loaded = 0;
            }
            else if (state == -3)
            {
                /*-------------------------------------------------Exit------------------------------------------------------------------------*/
                loaded = 0;
            }
        }
        else if (in == 'L')
        {
            /*-------------------------------------------------Load------------------------------------------------------------------------*/
            // Take the user name & check it
            char playerName[NAMELEN+1];
            center("Enter Your Name : ");
            gets(playerName);
            for (i=0; i<strlen(playerName); i++)
            {
                playerName[i] = tolower(playerName[i]);

            }
            FILE* loadFile = fopen(playerName,"r");
            if(loadFile == 0)
            {
                center("Error\n");
                center("Press Enter to Continue  ");
                gets(wrongchoice);
            }
            else
            {
                // Load the game & draw it in a file
                int i,n;
                fscanf(loadFile,"%d\nS",&n);
                for(i = 0; i < n; i++)
                {
                    disGrid[i] = fgetc(loadFile);
                    grid[i] = fgetc(loadFile);
                }
                curGame.disGrid = disGrid;
                curGame.grid = grid;
                time_t timeNow = time(0);   // get time no
                int timeInSec;
                fscanf(loadFile,"%d\n%d\n%d\n%d\n%d\n%d\n",&(curGame.height),&(curGame.width),&(curGame.numFlags),&(curGame.numMoves),&(curGame.numQues),&(timeInSec));
                curGame.startedTime = time(0) - timeInSec;
                fclose(loadFile);
                drawInFile(curGame);
                loaded = 1;
            }
        }
        else if (in == 'R')
        {
            /*-------------------------------------------------Game instructions------------------------------------------------------------*/

            center("You are presented with a board of squares.\n");
            center("Some squares contain mines (bombs), others don't.\n");
            center("If you open a square containing a bomb, you lose.\n");
            center("If you manage to click all the squares (without opening any bombs) you win.\n");
            center("Clicking a square which doesn't have a bomb reveals the number of neighbouring squares containing bombs.\n");
            center("Use this information plus some guess work to avoid the bombs.\n");
            center("To open a square, point at the square and press O. To mark a square you think is a bomb, point and press F ).");
            printf("\n\n");
            center("Closed cell : X\n");
            center("Open cell that is empty : ' '\n");
            center("Open cell with a number from 1 to 8 : the number in this cell\n");
            center("Cell with a flag : F\n");
            center("Cell with question mark : ?\n\n");
            center("Press Enter To Continue...  ");
            gets(wrongchoice);

        }
        else if (in == 'P')
        {
            /*-------------------------------------------------Players List----------------------------------------------------------------*/
            // Load the player's list
            int numOfPlayers;
            player* playerList;
            FILE* playerFileRead = fopen("playerList","r");
            if(playerFileRead == 0)     //If the file doesn't exist
            {
                center("Error\n");
            }
            else
            {
                // Scan the player's number
                fscanf(playerFileRead,"%d\n",&numOfPlayers);

                // Make an array for the players
                playerList = malloc(sizeof(player) * numOfPlayers);

                // Scan player's one by one
                for(i = 0; i < numOfPlayers; i++)
                {
                    fgets(playerList[i].name,NAMELEN,playerFileRead);
                    fscanf(playerFileRead,"%d %d\n",&playerList[i].score,&playerList[i].highestScore);
                    playerList[i].name[strlen(playerList[i].name) - 1]= '\0';
                }

                // Sort the player's list
                sortPlayerList(playerList,numOfPlayers);

                // Close the file
                fclose(playerFileRead);

                // Print the player's list (centered)
                printf("\n\n");
                for(i = 0; i < numOfPlayers; i++)
                {

                    sprintf(printstr,"%d.Player Name : %s\n",i+1,playerList[i].name);
                    center(printstr);
                    sprintf(printstr,"Total Score : %d      The Highest Score : %d\n",playerList[i].score,playerList[i].highestScore);
                    center(printstr);
                    center("\n\n");
                }

                // Free the array
                free(playerList);
                printf("\n\n");
            }
            center("Press Enter to Continue  ");
                gets(wrongchoice);
        }
    }

    // Done
    printf("\n\n");
    exitMSG();
}

