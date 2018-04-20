#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <windowsx.h>

#include <tchar.h>
#include <windows.h>

#define NAMELEN 100
#define MAXGRIDSIZE 900
#define SPACE 10

HWND mainWn;               // This is the handle for the main window
HWND startWn;              // This is the handle for the start window
HWND getDimWn;              // This is the handle for the get-dimensions window
HWND enterNameWn;              // This is the handle for the enter-name window
HWND playerListWn;

HWND startWnBt[5];
HWND getDimWnBt;
HWND maingameWnBt[MAXGRIDSIZE];
HWND mainsaveWnBt;
HWND mainexitWnBt;
HWND mainhintWnBt;
HWND enterNameWnBt;

HWND mainopenWnRBt;
HWND mainflagWnRBt;
HWND mainquestionWnRBt;
HWND mainunmarkWnRBt;

HWND heightTB;
HWND widthTB;
HWND nameTB;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure4Start (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WindowProcedure4Main (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WindowProcedure4GetDim (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WindowProcedure4EnterName (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WindowProcedure4Playlist (HWND, UINT, WPARAM, LPARAM);


HINSTANCE globalHINSTANCE;
int globalnCmdShow;

typedef struct game
{
    char* disGrid;              //Displayed Grid
    char* grid;                 //The Opened Grid
    int width;                  //The Width of the grid
    int height;                 //The Height of the grid
    int gridSize;               //The Size of the Grid (= height * width)
    int numFlags;               //Number of Flags in the grid  0
    int numMoves;               //Number of Moves 0
    int numQues;                //Number of Question marks in the grid 0
    time_t startedTime;         //Stared time in seconds
} game;
typedef struct player
{
    char name[NAMELEN];         //The player name
    int score;                  //The player total score
    int highestScore;           //The player highest score
} player;
int twoDArr(int i,int j,int height,int width);
//Get i & j for 2d array & return it's position in 1D array
void initialization(int firstCell); //The first move
//Put the mines & Set cells numbers & save the grid to a file
int numOfXInArr(int height,int width,int row,int col,char x,char* arr);
//Return number of neighbor (X) in the Arr
int openCell(int row,int col,int* viewedGrid);
//Open a cell and return state : 1 for win & 0 for normal & -1 for lose
void drawInFile();
//draw The opened grid if file to debug
void sortPlayerList(player* playerList,int numOfPlayers);
void hint(char* disGrid,int height,int width);
void twoDArrI(int n,int* i,int* j,int height,int width);
int executeOpen(int row,int col);
void showStartWn(void);

game curGame;
char disGrid[MAXGRIDSIZE];
char grid[MAXGRIDSIZE];

float score;
int winORsaveORload; //1 for win 2 for save 3 for load
int destroyEnterName;
int destroyStart;
int destroyGetDim;

char heightTBIN[3],widthTBIN[3],nameTBIN[NAMELEN];

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    FreeConsole();
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX startcl;        /* Data structure for the windowclass */
    WNDCLASSEX getDimcl;
    WNDCLASSEX maincl;        /* Data structure for the windowclass */
    WNDCLASSEX enterNamecl;
    WNDCLASSEX playerListcl;

    globalHINSTANCE = hThisInstance;
    globalnCmdShow = nCmdShow;

    /* The Window structure */
    startcl.hInstance = hThisInstance;
    startcl.lpszClassName = "StartWn";
    startcl.lpfnWndProc = WindowProcedure4Start;      /* This function is called by windows */
    startcl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    startcl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    startcl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    startcl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    startcl.hCursor = LoadCursor (NULL, IDC_ARROW);
    startcl.lpszMenuName = NULL;                 /* No menu */
    startcl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    startcl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    startcl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&startcl))
        return 0;

    /* The Window structure */
    playerListcl.hInstance = hThisInstance;
    playerListcl.lpszClassName = "PlayerListWn";
    playerListcl.lpfnWndProc = WindowProcedure4Playlist;      /* This function is called by windows */
    playerListcl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    playerListcl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    playerListcl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    playerListcl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    playerListcl.hCursor = LoadCursor (NULL, IDC_ARROW);
    playerListcl.lpszMenuName = NULL;                 /* No menu */
    playerListcl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    playerListcl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    playerListcl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&playerListcl))
        return 0;

    /* The Window structure */
    getDimcl.hInstance = globalHINSTANCE;
    getDimcl.lpszClassName = "GetDimWn";
    getDimcl.lpfnWndProc = WindowProcedure4GetDim;      /* This function is called by windows */
    getDimcl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    getDimcl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    getDimcl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    getDimcl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    getDimcl.hCursor = LoadCursor (NULL, IDC_ARROW);
    getDimcl.lpszMenuName = NULL;                 /* No menu */
    getDimcl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    getDimcl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    getDimcl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&getDimcl))
        return 0;
    /* The Window structure */
    enterNamecl.hInstance = globalHINSTANCE;
    enterNamecl.lpszClassName = "enterNameWn";
    enterNamecl.lpfnWndProc = WindowProcedure4EnterName;      /* This function is called by windows */
    enterNamecl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    enterNamecl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    enterNamecl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    enterNamecl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    enterNamecl.hCursor = LoadCursor (NULL, IDC_ARROW);
    enterNamecl.lpszMenuName = NULL;                 /* No menu */
    enterNamecl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    enterNamecl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    enterNamecl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&enterNamecl))
        return 0;
    /* The Window structure */
    maincl.hInstance = globalHINSTANCE;
    maincl.lpszClassName = "MainWn";
    maincl.lpfnWndProc = WindowProcedure4Main;      /* This function is called by windows */
    maincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    maincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    maincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    maincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    maincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    maincl.lpszMenuName = NULL;                 /* No menu */
    maincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    maincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    maincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&maincl))
        return 0;
    /* The class is registered, let's create the program*/
    showStartWn();

    /* Make the window visible on the screen */


    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage(&messages,NULL,0,0))
    {

        TranslateMessage(&messages);
        DispatchMessage(&messages);


    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure4Start (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        char choices[5][NAMELEN];
    case WM_SHOWWINDOW:
        destroyStart = 0;
        strcpy(choices[0],"New Game");
        strcpy(choices[1],"Load Game");
        strcpy(choices[2],"Read Game Instructions");
        strcpy(choices[3],"Show Player's List");
        strcpy(choices[4],"Exit");
        int i;
        for(i = 0; i < 5; i++)
        {
            startWnBt[0] = CreateWindow("BUTTON",
                                        choices[i],
                                        WS_CHILD | WS_BORDER | WS_VISIBLE,
                                        SPACE,
                                        (SPACE+50)*i+SPACE,
                                        385,
                                        50,
                                        hwnd,i+1,NULL,NULL);
        }
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        /* Data structure for the windowclass */
        case 1:
            getDimWn = CreateWindowEx (
                           0,                   /* Extended possibilites for variation */
                           "GetDimWn",         /* Classname */
                           "Minesweeper",       /* Title Text */
                           WS_SYSMENU | WS_MINIMIZEBOX, /* default window */
                           CW_USEDEFAULT,       /* Windows decides the position */
                           CW_USEDEFAULT,       /* where the window ends up on the screen */
                           300,                 /* The programs width */
                           200,                 /* and height in pixels */
                           HWND_DESKTOP,        /* The window is a child-window to desktop */
                           NULL,                /* No menu */
                           globalHINSTANCE,       /* Program Instance handler */
                           NULL                 /* No Window Creation data */
                       );
            ShowWindow (getDimWn, globalnCmdShow);
            destroyStart = 1;
            DestroyWindow(hwnd);
            break;
        case 2:
            winORsaveORload = 3;
            enterNameWn = CreateWindowEx (
                              0,                   /* Extended possibilites for variation */
                              "enterNameWn",         /* Classname */
                              "Minesweeper!!",       /* Title Text */
                              WS_SYSMENU | WS_MINIMIZEBOX, /* default window */
                              CW_USEDEFAULT,       /* Windows decides the position */
                              CW_USEDEFAULT,       /* where the window ends up on the screen */
                              300,
                              150,                 /* and height in pixels */
                              HWND_DESKTOP,        /* The window is a child-window to desktop */
                              NULL,                /* No menu */
                              globalHINSTANCE,       /* Program Instance handler */
                              NULL                 /* No Window Creation data */
                          );
            ShowWindow (enterNameWn, globalnCmdShow);
            destroyStart = 1;
            DestroyWindow(hwnd);
        case 3:
            MessageBeep(MB_ICONASTERISK);
            char help[1000];
            strcpy(help,"You are presented with a board of squares. Some squares contain mines (bombs), others don't. If you click on a square containing a bomb, you lose. If you manage to click all the squares (without clicking on any bombs) you win.\nClicking a square which doesn't have a bomb reveals the number of neighbouring squares containing bombs. Use this information plus some guess work to avoid the bombs.\nTo open a square, point at the square and press O. To mark a square you think is a bomb, point and press F ).\n\n\n\n\n\t. Closed cell : X\n\n\t. Open cell that is empty : ' '\n\n\t. Open cell with a number from 1 to 8 : the number in this cell\n\n\t. Cell with a flag : F\n\n\t. Cell with question mark : ?\n");
            MessageBox(hwnd,help,"Game Instructions",MB_OK);
            break;
        case 4:

            playerListWn = CreateWindowEx (
                               0,                   /* Extended possibilites for variation */
                               "PlayerListWn",         /* Classname */
                               _T("Welcome to Minesweeper!!"),       /* Title Text */
                               WS_SYSMENU | WS_MINIMIZEBOX, /* default window */
                               CW_USEDEFAULT,       /* Windows decides the position */
                               CW_USEDEFAULT,       /* where the window ends up on the screen */
                               410,                 /* The programs width */
                               340,                 /* and height in pixels */
                               HWND_DESKTOP,        /* The window is a child-window to desktop */
                               NULL,                /* No menu */
                               globalHINSTANCE,       /* Program Instance handler */
                               NULL                 /* No Window Creation data */
                           );

            /* Make the window visible on the screen */
            ShowWindow (playerListWn, globalnCmdShow);
            break;
        case 5:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        }
        break;
    case WM_DESTROY:
        if(!destroyStart)
            PostQuitMessage(0);
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
LRESULT CALLBACK WindowProcedure4GetDim (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
    case WM_SHOWWINDOW:
        CreateWindow("STATIC",
                     "Enter The Height :",
                     WS_CHILD | WS_VISIBLE | SS_CENTER,
                     SPACE,SPACE,275,20,
                     hwnd,NULL,NULL,NULL);
        heightTB = CreateWindow("EDIT",
                                "",
                                WS_CHILD | WS_VISIBLE | WS_VISIBLE | ES_CENTER,
                                SPACE,SPACE*2+20,275,20,
                                hwnd,NULL,NULL,NULL);
        CreateWindow("STATIC",
                     "Enter The Width :",
                     WS_CHILD | WS_VISIBLE | SS_CENTER,
                     SPACE,SPACE*3+20*2,275,20,
                     hwnd,NULL,NULL,NULL);
        widthTB = CreateWindow("EDIT",
                               "",
                               WS_CHILD | WS_VISIBLE | WS_VISIBLE | ES_CENTER,
                               SPACE,SPACE*4+20*3,275,20,
                               hwnd,NULL,NULL,NULL);

        getDimWnBt = CreateWindow("BUTTON",
                                  "Start",
                                  WS_CHILD | WS_BORDER | WS_VISIBLE,
                                  SPACE,SPACE*5+20*4,275,30,
                                  hwnd,1,NULL,NULL);
        destroyGetDim = 0;
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
            int i;
        case 1:
            GetWindowText(heightTB,heightTBIN,3);
            GetWindowText(widthTB,widthTBIN,3);
            ////////////////////////////////////Check the input
            curGame.height = atoi(heightTBIN);
            curGame.width = atoi(widthTBIN);
            curGame.gridSize = atoi(heightTBIN) * atoi(widthTBIN);
            curGame.numFlags = 0;
            curGame.numMoves = 0;
            curGame.numQues = 0;
            curGame.disGrid = disGrid;
            curGame.grid = grid;
            for(i = 0; i < curGame.gridSize; i++)
            {
                disGrid[i] = 'X';
            }
            destroyGetDim = 1;
            DestroyWindow(hwnd);
            mainWn = CreateWindowEx (
                         0,                   /* Extended possibilites for variation */
                         "MainWn",         /* Classname */
                         "Minesweeper",       /* Title Text */
                         WS_SYSMENU | WS_MINIMIZEBOX, /* default window */
                         CW_USEDEFAULT,       /* Windows decides the position */
                         CW_USEDEFAULT,       /* where the window ends up on the screen */
                         curGame.width  * 22 + 3 * SPACE + (curGame.width-1) * SPACE/2,                 /* The programs width */
                         20+curGame.height * 22 + 3 * SPACE + (curGame.height-1) * SPACE/2 + 230,                 /* and height in pixels */
                         HWND_DESKTOP,        /* The window is a child-window to desktop */
                         NULL,                /* No menu */
                         globalHINSTANCE,       /* Program Instance handler */
                         NULL                 /* No Window Creation data */
                     );
            ShowWindow (mainWn, globalnCmdShow);

        }
        break;
    case WM_DESTROY:
        if(!destroyGetDim)
            showStartWn();
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
LRESULT CALLBACK WindowProcedure4Main (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)                  /* handle the messages */
    {
        int i,j,x,row,col,state = 0;
        char str[2];
    case WM_SHOWWINDOW:
        curGame.startedTime = time(0);
        for(i = 0; i < curGame.height; i++)
        {
            for(j = 0; j < curGame.width; j++)
            {
                x = twoDArr(i,j,curGame.height,curGame.width);
                str[0] = curGame.disGrid[x];
                str[1] = 0;
                maingameWnBt[x] = CreateWindow("BUTTON",
                                               str,
                                               WS_CHILD | WS_BORDER | WS_VISIBLE | BS_CENTER,
                                               SPACE + SPACE/2 * j + j * 22,
                                               SPACE + SPACE/2 * i + i * 22,
                                               20,20,hwnd,x+1,NULL,NULL);
            }
            mainopenWnRBt = CreateWindow("BUTTON",
                                         "Open",
                                         WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
                                         SPACE,
                                         SPACE* 3  + curGame.height * 22 + (curGame.height-1) * SPACE/2,
                                         100,
                                         20,hwnd,(HMENU)curGame.gridSize+11,NULL,NULL);
            mainflagWnRBt = CreateWindow("BUTTON",
                                         "Flag",
                                         WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
                                         SPACE,
                                         SPACE*4 + 20  + curGame.height * 22 + (curGame.height-1) * SPACE/2,
                                         100,
                                         20,hwnd,(HMENU)curGame.gridSize+12,NULL,NULL);
            mainquestionWnRBt = CreateWindow("BUTTON",
                                             "Question",
                                             WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
                                             SPACE,
                                             SPACE*5 + 20*2 + curGame.height  * 22 + (curGame.height-1) * SPACE/2,
                                             100,
                                             20,hwnd,(HMENU)curGame.gridSize+13,NULL,NULL);
            mainunmarkWnRBt = CreateWindow("BUTTON",
                                           "Unmark",
                                           WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
                                           SPACE,
                                           SPACE*6 + 20*3  + curGame.height  * 22 + (curGame.height-1) * SPACE/2,
                                           100,
                                           20,hwnd,(HMENU)curGame.gridSize+14,NULL,NULL);
            SendMessage(mainopenWnRBt,BM_SETCHECK, BST_CHECKED, 0);
            mainhintWnBt = CreateWindow("BUTTON",
                                        "Hint",
                                        WS_CHILD | WS_BORDER | WS_VISIBLE | BS_CENTER,
                                        SPACE,
                                        SPACE*7 + 20*4  + curGame.height  * 22 + (curGame.height-1) * SPACE/2,
                                        curGame.width  * 22 +  SPACE + (curGame.width-1) * SPACE/2,
                                        20,hwnd,(HMENU)curGame.gridSize+21,NULL,NULL);
            mainsaveWnBt = CreateWindow("BUTTON",
                                        "Save",
                                        WS_CHILD | WS_BORDER | WS_VISIBLE | BS_CENTER,
                                        SPACE,
                                        SPACE*8 + 20*5  + curGame.height  * 22 + (curGame.height-1) * SPACE/2,
                                        curGame.width  * 22 +  SPACE + (curGame.width-1) * SPACE/2,
                                        20,hwnd,(HMENU)curGame.gridSize+22,NULL,NULL);
            mainexitWnBt = CreateWindow("BUTTON",
                                        "Exit",
                                        WS_CHILD | WS_BORDER | WS_VISIBLE | BS_CENTER,
                                        SPACE,
                                        SPACE*9 + 20*6  + curGame.height  * 22 + (curGame.height-1) * SPACE/2,
                                        curGame.width  * 22 +  SPACE + (curGame.width-1) * SPACE/2,
                                        20,hwnd,(HMENU)curGame.gridSize+23,NULL,NULL);

        }
        break;
    case WM_COMMAND:
        if(LOWORD(wParam) > 0 && LOWORD(wParam) < curGame.gridSize+10 && curGame.disGrid[LOWORD(wParam) - 1] != ' ')
        {
            if(SendMessage(mainopenWnRBt,BM_GETCHECK, 0, 0) == BST_CHECKED)
            {
                if(curGame.numMoves == 0)
                {
                    curGame.startedTime = time(0);
                    curGame.numMoves++;
                    initialization(LOWORD(wParam) - 1);
                    for(i = 0; i < curGame.gridSize; i++)
                    {
                        str[0] = curGame.disGrid[i];
                        str[1] = 0;
                        SendMessage(maingameWnBt[i],WM_SETTEXT, 0,str);
                    }
                }
                else
                {
                    curGame.numMoves++;
                    twoDArrI(LOWORD(wParam) - 1,&row,&col,curGame.height,curGame.width);
                    state = executeOpen(row,col);
                    for(i = 0; i < curGame.gridSize; i++)
                    {
                        str[0] = curGame.disGrid[i];
                        str[1] = 0;
                        SendMessage(maingameWnBt[i],WM_SETTEXT, 0,str);
                    }
                    if(state == 1)
                    {
                        winORsaveORload = 1;
                        time_t timeNow = time(0);   // get time no
                        int timeInSec = (timeNow - curGame.startedTime) % 60;
                        int timeInMin = (timeNow - curGame.startedTime) / 60;
                        score = (pow(curGame.height,4)*pow(curGame.width,4))/(((timeInMin*60)+(timeInSec%60))*curGame.numMoves);
                        char msg[100];
                        sprintf(msg,"Congratulations\nYour score is %d",(int)score);
                        MessageBeep(MB_ICONASTERISK);
                        MessageBox(hwnd,msg,"Win",MB_OK);
                        DestroyWindow(hwnd);
                        enterNameWn = CreateWindowEx (
                                          0,                   /* Extended possibilites for variation */
                                          "enterNameWn",         /* Classname */
                                          "Minesweeper!!",       /* Title Text */
                                          WS_SYSMENU | WS_MINIMIZEBOX, /* default window */
                                          CW_USEDEFAULT,       /* Windows decides the position */
                                          CW_USEDEFAULT,       /* where the window ends up on the screen */
                                          300,
                                          150,                 /* and height in pixels */
                                          HWND_DESKTOP,        /* The window is a child-window to desktop */
                                          NULL,                /* No menu */
                                          globalHINSTANCE,       /* Program Instance handler */
                                          NULL                 /* No Window Creation data */
                                      );
                        ShowWindow (enterNameWn, globalnCmdShow);
                    }
                    else if(state == -1)
                    {
                        MessageBeep(MB_ICONERROR);
                        MessageBox(hwnd,"You Have Lost !! ","Lose",MB_OK);
                        DestroyWindow(hwnd);
                    }
                }
            }
            else if(SendMessage(mainflagWnRBt,BM_GETCHECK, 0, 0) == BST_CHECKED  && (curGame.disGrid[LOWORD(wParam) - 1] == 'X' || curGame.disGrid[LOWORD(wParam) - 1] == '?' || curGame.disGrid[LOWORD(wParam) - 1] == 'F'))
            {
                if (curGame.disGrid[LOWORD(wParam) - 1] == 'F')
                    curGame.disGrid[LOWORD(wParam) - 1] = 'X';
                else
                    curGame.disGrid[LOWORD(wParam) - 1] = 'F';
                for(i = 0; i < curGame.gridSize; i++)
                {
                    str[0] = curGame.disGrid[i];
                    str[1] = 0;
                    SendMessage(maingameWnBt[i],WM_SETTEXT, 0,str);
                }
            }
            else if(SendMessage(mainquestionWnRBt,BM_GETCHECK, 0, 0) == BST_CHECKED && (curGame.disGrid[LOWORD(wParam) - 1] == 'X' || curGame.disGrid[LOWORD(wParam) - 1] == '?' || curGame.disGrid[LOWORD(wParam) - 1] == 'F'))
            {
                if (curGame.disGrid[LOWORD(wParam) - 1] == '?')
                    curGame.disGrid[LOWORD(wParam) - 1] = 'X';
                else
                    curGame.disGrid[LOWORD(wParam) - 1] = '?';
                for(i = 0; i < curGame.gridSize; i++)
                {
                    str[0] = curGame.disGrid[i];
                    str[1] = 0;
                    SendMessage(maingameWnBt[i],WM_SETTEXT, 0,str);
                }
            }
            else if(SendMessage(mainunmarkWnRBt,BM_GETCHECK, 0, 0 && (curGame.disGrid[LOWORD(wParam) - 1] == 'F' || curGame.disGrid[LOWORD(wParam) - 1] == '?')) == BST_CHECKED)
            {
                curGame.disGrid[LOWORD(wParam) - 1] = 'X';
                for(i = 0; i < curGame.gridSize; i++)
                {
                    str[0] = curGame.disGrid[i];
                    str[1] = 0;
                    SendMessage(maingameWnBt[i],WM_SETTEXT, 0,str);
                }
            }
        }
        else if (LOWORD(wParam) == (HMENU)curGame.gridSize + 11)
        {
            SendMessage(mainopenWnRBt,BM_SETCHECK, BST_CHECKED, 0);
            SendMessage(mainflagWnRBt,BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(mainquestionWnRBt,BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(mainunmarkWnRBt,BM_SETCHECK, BST_UNCHECKED, 0);
        }
        else if (LOWORD(wParam) == (HMENU)curGame.gridSize + 12)
        {
            SendMessage(mainopenWnRBt,BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(mainflagWnRBt,BM_SETCHECK, BST_CHECKED, 0);
            SendMessage(mainquestionWnRBt,BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(mainunmarkWnRBt,BM_SETCHECK, BST_UNCHECKED, 0);
        }
        else if (LOWORD(wParam) == (HMENU)curGame.gridSize + 13)
        {
            SendMessage(mainopenWnRBt,BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(mainflagWnRBt,BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(mainquestionWnRBt,BM_SETCHECK, BST_CHECKED, 0);
            SendMessage(mainunmarkWnRBt,BM_SETCHECK, BST_UNCHECKED, 0);
        }
        else if (LOWORD(wParam) == (HMENU)curGame.gridSize + 14)
        {
            SendMessage(mainopenWnRBt,BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(mainflagWnRBt,BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(mainquestionWnRBt,BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(mainunmarkWnRBt,BM_SETCHECK, BST_CHECKED, 0);
        }
        else if (LOWORD(wParam) == (HMENU)curGame.gridSize + 21)
        {
            hint(curGame.disGrid,curGame.height,curGame.width);
        }
        else if (LOWORD(wParam) == (HMENU)curGame.gridSize + 22)
        {
            winORsaveORload = 2;
            DestroyWindow(hwnd);
            enterNameWn = CreateWindowEx (
                              0,                   /* Extended possibilites for variation */
                              "enterNameWn",         /* Classname */
                              "Minesweeper!!",       /* Title Text */
                              WS_SYSMENU | WS_MINIMIZEBOX, /* default window */
                              CW_USEDEFAULT,       /* Windows decides the position */
                              CW_USEDEFAULT,       /* where the window ends up on the screen */
                              300,
                              150,                 /* and height in pixels */
                              HWND_DESKTOP,        /* The window is a child-window to desktop */
                              NULL,                /* No menu */
                              globalHINSTANCE,       /* Program Instance handler */
                              NULL                 /* No Window Creation data */
                          );
            ShowWindow (enterNameWn, globalnCmdShow);
        }
        else if (LOWORD(wParam) == (HMENU)curGame.gridSize + 23)
        {
            DestroyWindow(hwnd);
        }
        break;
    case WM_DESTROY:
        showStartWn();
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
LRESULT CALLBACK WindowProcedure4EnterName (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
    case WM_SHOWWINDOW:
        destroyEnterName = 0;
        CreateWindow("STATIC",
                     "Enter Your Name",
                     WS_CHILD | WS_VISIBLE | SS_CENTER,
                     SPACE,SPACE*0+20*1,275,20,
                     hwnd,NULL,NULL,NULL);
        nameTB = CreateWindow("EDIT",
                              "",
                              WS_CHILD | WS_VISIBLE | WS_VISIBLE | ES_CENTER,
                              SPACE,SPACE*1+20*2,275,20,
                              hwnd,NULL,NULL,NULL);

        enterNameWnBt = CreateWindow("BUTTON",
                                     "OK",
                                     WS_CHILD | WS_BORDER | WS_VISIBLE,
                                     SPACE,SPACE*2+20*3,275,30,
                                     hwnd,1,NULL,NULL);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
            int i;
        case 1:
            GetWindowText(nameTB,nameTBIN,NAMELEN+1);
            for (i=0; i<strlen(nameTBIN); i++)
            {
                nameTBIN[i]=tolower(nameTBIN[i]);
            }
            if(winORsaveORload == 1)
            {
                int numOfPlayers;
                FILE* playerFileRead = fopen("playerList","r");
                int flag = 1;
                player* playerList;
                if(playerFileRead == 0)
                {
                    numOfPlayers = 1;
                    playerList = malloc(sizeof(player));
                    playerList->highestScore = playerList->score = score;
                    strcpy(playerList->name,nameTBIN);
                }
                else
                {
                    fscanf(playerFileRead,"%d\n",&numOfPlayers);
                    playerList = malloc(sizeof(player) * (numOfPlayers+1));
                    for(i = 0; i < numOfPlayers; i++)
                    {
                        fgets(playerList[i].name,NAMELEN,playerFileRead);
                        fscanf(playerFileRead,"%d %d\n",&playerList[i].score,&playerList[i].highestScore);
                        playerList[i].name[strlen(playerList[i].name) - 1]= '\0';
                        if(strcmp(playerList[i].name,nameTBIN) == 0)
                        {
                            playerList[i].score+=score;
                            if(score > playerList[i].highestScore)
                            {
                                playerList[i].highestScore = score;
                            }
                            flag = 0;
                        }
                    }
                    if(flag)
                    {
                        playerList[numOfPlayers].highestScore = playerList[numOfPlayers].score = score;
                        strcpy(playerList[numOfPlayers].name,nameTBIN);
                        numOfPlayers++;
                    }

                }
                fclose(playerFileRead);
                FILE* playerFileWrite = fopen("playerList","w");
                fprintf(playerFileWrite,"%d\n",numOfPlayers);
                for(i = 0; i < numOfPlayers; i++)
                {
                    fprintf(playerFileWrite,"%s\n",playerList[i].name);
                    fprintf(playerFileWrite,"%d %d\n",playerList[i].score,playerList[i].highestScore);
                }
                fclose(playerFileWrite);
                free(playerList);
                destroyEnterName = 1;
                DestroyWindow(hwnd);
            }
            else if(winORsaveORload == 2)
            {
                FILE* saveFile = fopen(nameTBIN,"w");
                fprintf(saveFile,"%d\nS",curGame.gridSize);
                for(i = 0; i < curGame.gridSize; i++)
                {
                    fprintf(saveFile,"%c%c",curGame.disGrid[i],curGame.grid[i]);
                }
                fprintf(saveFile,"\n");
                time_t timeNow = time(0);   // get time no
                int timeInSec = (timeNow - curGame.startedTime);
                fprintf(saveFile,"%d\n%d\n%d\n%d\n%d\n%d\n",curGame.height,curGame.width,curGame.numFlags,curGame.numMoves,curGame.numQues,timeInSec);
                fclose(saveFile);
                MessageBeep(MB_ICONASTERISK);
                MessageBox(hwnd,"Game Successfully Saved !!","Saved",MB_OK);
                destroyEnterName = 1;
                DestroyWindow(hwnd);
            }
            else if(winORsaveORload == 3)
            {
                FILE* loadFile = fopen(nameTBIN,"r");
                if(loadFile == 0)
                {
                    MessageBeep(MB_ICONERROR);
                    MessageBox(hwnd,"The Name is Wrong !!","Error",MB_OK);
                }
                else
                {
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
                    mainWn = CreateWindowEx (
                                 0,                   /* Extended possibilites for variation */
                                 "MainWn",         /* Classname */
                                 "Minesweeper",       /* Title Text */
                                 WS_SYSMENU | WS_MINIMIZEBOX, /* default window */
                                 CW_USEDEFAULT,       /* Windows decides the position */
                                 CW_USEDEFAULT,       /* where the window ends up on the screen */
                                 curGame.width  * 22 + 3 * SPACE + (curGame.width-1) * SPACE/2,                 /* The programs width */
                                 20+curGame.height * 22 + 3 * SPACE + (curGame.height-1) * SPACE/2 + 230,                 /* and height in pixels */
                                 HWND_DESKTOP,        /* The window is a child-window to desktop */
                                 NULL,                /* No menu */
                                 globalHINSTANCE,       /* Program Instance handler */
                                 NULL                 /* No Window Creation data */
                             );
                    ShowWindow (mainWn, globalnCmdShow);
                    destroyEnterName = 1;
                    DestroyWindow(hwnd);
                }
            }
            break;
        }
        break;
    case WM_DESTROY:
        if(!destroyEnterName)
        {
            if(winORsaveORload == 1 || winORsaveORload == 3)
                showStartWn();
        }
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
LRESULT CALLBACK WindowProcedure4Playlist (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        int i;
        int numOfPlayers;
        char* msg;
        player* playerList;
        FILE* playerFileRead;
    case WM_SHOWWINDOW:
        playerFileRead = fopen("playerList","r");
        if(playerFileRead == 0)
        {
            msg = malloc(1000);
            strcpy(msg,"There isn't a player list yet!! ");
        }
        else
        {
            msg = malloc(numOfPlayers*1000);
            strcpy(msg,"");
            fscanf(playerFileRead,"%d\n",&numOfPlayers);
            char playerListPrint[numOfPlayers * 1000];
            playerList = malloc(sizeof(player) * numOfPlayers);
            for(i = 0; i < numOfPlayers; i++)
            {
                fgets(playerList[i].name,NAMELEN,playerFileRead);
                fscanf(playerFileRead,"%d %d\n",&playerList[i].score,&playerList[i].highestScore);
                playerList[i].name[strlen(playerList[i].name) - 1]= '\0';
            }
            sortPlayerList(playerList,numOfPlayers);
            fclose(playerFileRead);
            char tmp[1000];
            for(i = 0; i < numOfPlayers; i++)
            {

                sprintf(tmp,"%d.Player Name : %s\r\nTotal Score : %d\t The Highest Score : %d\r\n\r\n",i+1,playerList[i].name,playerList[i].score,playerList[i].highestScore);
                strcat(msg,tmp);
            }
            free(playerList);
        }
        CreateWindow("EDIT",
                     msg,
                     WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | WS_VSCROLL | ES_MULTILINE | ES_CENTER,
                     SPACE,SPACE,380,290,
                     hwnd,NULL,NULL,NULL);
        free(msg);
        break;
    case WM_DESTROY:
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);

    }
    return 0;
}
int twoDArr(int i,int j,int height,int width)
{
    return i + j * height;
}
void twoDArrI(int n,int* i,int* j,int height,int width)
{

    *i = n % height;
    *j = n / height;
}
void initialization(int firstCell)
{
    int i,j;
    int row,col;
    int viewedGrid[curGame.height * curGame.width];
    //do
    //{
    int minesNum = 1 + (curGame.height * curGame.width) / 10;

    for(i = 0; i < curGame.height * curGame.width; i++)
    {
        viewedGrid[i] = 0;
    }

    int sum = 0;
    for(i = 0; i < curGame.height; i++)
    {
        for(j =0; j < curGame.width; j++)
        {
            curGame.grid[twoDArr(i,j,curGame.height,curGame.width)] = ' ';
        }
    }
    int pos; // position of mines
    for(i = 0; i < minesNum; i++)
    {
        do
        {
            srand(rand()*rand()*time(0));
            pos = rand()%(curGame.height*curGame.width);
        }
        while(pos == firstCell || curGame.grid[pos] == '*' );
        curGame.grid[pos] = '*';
    }
    //Put numbers in cell
    int minesNumInCell;
    for (i=0; i<curGame.height; i++)
    {
        for (j=0; j<curGame.width; j++)
        {
            if (curGame.grid[twoDArr(i,j,curGame.height,curGame.width)] != '*')
            {
                minesNumInCell = numOfXInArr(curGame.height,curGame.width,i,j,'*',curGame.grid);
                if (minesNumInCell != 0)
                    curGame.grid[twoDArr(i,j,curGame.height,curGame.width)] = minesNumInCell + '0';
                else
                    curGame.grid[twoDArr(i,j,curGame.height,curGame.width)] =' ';
            }
        }
    }
    drawInFile();
    twoDArrI(firstCell,&row,&col,curGame.height,curGame.width);
    openCell(row,col,viewedGrid);
    //}
    //while();
}

void drawInFile()
{
    int i,j;
    FILE * fGrid;
    fGrid = fopen("grid.txt", "w");
    fprintf(fGrid, "    ");
    for(i = 0; i < curGame.width; i++)
    {
        if (i<=8)
        {
            fprintf(fGrid, "  %d ",i+1);
        }
        else
        {
            fprintf(fGrid, " %d ",i+1);
        }
    }
    fprintf(fGrid,"\n    +");
    for(i = 0; i < curGame.width; i++)
        fprintf(fGrid,"---+");
    fprintf(fGrid,"\n");
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
        fprintf(fGrid, "    +");
        for(j = 0; j < curGame.width; j++)
            fprintf(fGrid, "---+");
        fprintf(fGrid, "\n");
    }
    fclose(fGrid);
}

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

int executeOpen(int row,int col)
{
    //make an array to use DFS and put it's value = 0
    int viewedGrid[curGame.gridSize];
    int i;
    for(i = 0; i < curGame.gridSize; i++)
    {
        viewedGrid[i] = 0;
    }
    //Open the cell if not open
    if (curGame.disGrid[twoDArr(row,col,curGame.height,curGame.width)] == 'X' || curGame.disGrid[twoDArr(row,col,curGame.height,curGame.width)] == '?')
    {
        return openCell(row,col,viewedGrid);
    }
    else if (curGame.disGrid[twoDArr(row,col,curGame.height,curGame.width)] >= '1' && curGame.disGrid[twoDArr(row,col,curGame.height,curGame.width)] <= '8' )
    {
        //Count the flags if the cell contain a number and open all the reachable cells from it if the flags = the number
        int numOfFlagsInCell = numOfXInArr(curGame.height,curGame.width,row,col,'F',curGame.disGrid);
        if (numOfFlagsInCell >= curGame.disGrid[twoDArr(row,col,curGame.height,curGame.width)]-'0')
        {
            int k,l,state = 0;
            for (k= row-1; k<=row+1; k++)
            {
                for (l=col-1; l<=col+1; l++)
                {
                    if (k < 0 || k >= curGame.height || l < 0 || l >= curGame.width)
                    {
                        continue;
                    }
                    if (curGame.disGrid[twoDArr(k,l,curGame.height,curGame.width)] != 'F')
                    {
                        //open neighbor cells till lose or win
                        state = openCell(k,l,viewedGrid);
                        if(state != 0)
                        {
                            return  state;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
int openCell(int row,int col,int* viewedGrid)
{
    int i,j;
    //Check for lose
    if(curGame.grid[twoDArr(row,col,curGame.height,curGame.width)] == '*')
    {
        // view unopened mines
        for (i=0; i<curGame.height; i++)
        {
            for (j=0; j<curGame.width; j++)
            {

                if (curGame.grid[twoDArr(i,j,curGame.height,curGame.width)]=='*' && curGame.disGrid[twoDArr(i,j,curGame.height,curGame.width)] != 'F')
                {
                    curGame.disGrid[twoDArr(i,j,curGame.height,curGame.width)] = 'M';
                }

                if (curGame.disGrid[twoDArr(i,j,curGame.height,curGame.width)] == 'F'  && curGame.grid[twoDArr(i,j,curGame.height,curGame.width)] != '*')
                {
                    curGame.disGrid[twoDArr(i,j,curGame.height,curGame.width)] = '_';
                }

            }
        }
        curGame.disGrid[twoDArr(row,col,curGame.height,curGame.width)] = '!';
        return -1;
    }
    viewedGrid[twoDArr(row,col,curGame.height,curGame.width)] = 1;
    //Open neighbor cells if the cell is empty
    if(curGame.grid[twoDArr(row,col,curGame.height,curGame.width)] == ' ')
    {
        for (i = row-1; i <= row+1; i++)
        {
            for (j = col-1; j <= col+1; j++)
            {
                if (i < 0|| i >= curGame.height || j < 0 || j >= curGame.width)
                {
                    continue;
                }
                if(viewedGrid[twoDArr(i,j,curGame.height,curGame.width)] == 0 && curGame.disGrid[twoDArr(i,j,curGame.height,curGame.width)] != 'F')
                {
                    openCell(i,j,viewedGrid);
                }
            }
        }
    }
    //Display the cell content
    curGame.disGrid[twoDArr(row,col,curGame.height,curGame.width)] = curGame.grid[twoDArr(row,col,curGame.height,curGame.width)];
    //Check for win
    int unOpenedCells = 0;
    for (i = 0; i < curGame.gridSize; i++)
    {
        if(curGame.disGrid[i] == '?' || curGame.disGrid[i] == 'X' || curGame.disGrid[i] == 'F')
        {
            unOpenedCells++;
        }
    }
    if(unOpenedCells == 1 + (curGame.height * curGame.width) / 10)
    {
        curGame.disGrid[twoDArr(row,col,curGame.height,curGame.width)] = curGame.grid[twoDArr(row,col,curGame.height,curGame.width)];
        return 1;
    }
    return 0;
}
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
void hint(char* disGrid,int height,int width)
{
    int i,j,k,l;
    char msg[100];
    //Prepare a array for probably of mines
    float probOfMine;
    int numOfXArountCell;
    for(i = 0; i < height; i++)
    {
        for(j = 0; j < width; j++)
        {
            if(disGrid[twoDArr(i,j,height,width)] >= '1' && disGrid[twoDArr(i,j,height,width)] <= '8')
            {
                //Calculate the probability of mine around a numbered cell
                numOfXArountCell = numOfXInArr(height,width,i,j,'X',disGrid) + numOfXInArr(height,width,i,j,'?',disGrid);
                probOfMine = (float)(disGrid[twoDArr(i,j,height,width)]-'0' - numOfXInArr(height,width,i,j,'F',disGrid)) / numOfXArountCell;
                //Add the probability to the array
                if(probOfMine == 0 && numOfXArountCell != 0)
                {
                    sprintf(msg,"You can open all cells (without flags) around (%d,%d)",i+1,j+1);
                    MessageBeep(MB_ICONASTERISK);
                    MessageBox(mainWn,msg,"Hint",MB_OK);
                    return;
                }
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
                            sprintf(msg,"The cell (%d,%d) is a mine",k+1,l+1);
                            MessageBeep(MB_ICONASTERISK);
                            MessageBox(mainWn,msg,"Hint",MB_OK);

                            return;
                        }
                    }
                }
            }
        }
    }
    sprintf(msg,"No Confirmed Hint!!");
    MessageBeep(MB_ICONASTERISK);
    MessageBox(mainWn,msg,"Hint",MB_OK);

}
void showStartWn(void)
{
    startWn = CreateWindowEx (
                  0,                   /* Extended possibilites for variation */
                  "StartWn",         /* Classname */
                  _T("Welcome to Minesweeper!!"),       /* Title Text */
                  WS_SYSMENU | WS_MINIMIZEBOX, /* default window */
                  CW_USEDEFAULT,       /* Windows decides the position */
                  CW_USEDEFAULT,       /* where the window ends up on the screen */
                  410,                 /* The programs width */
                  340,                 /* and height in pixels */
                  HWND_DESKTOP,        /* The window is a child-window to desktop */
                  NULL,                /* No menu */
                  globalHINSTANCE,       /* Program Instance handler */
                  NULL                 /* No Window Creation data */
              );
    ShowWindow (startWn, globalnCmdShow);
}
