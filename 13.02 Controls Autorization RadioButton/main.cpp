#include <windows.h>
#include "resource1.h"
#include <cstdlib>

HINSTANCE hInstance;
HWND buttons[9];
bool playerTurn = true; // true = крестик,  false = нолик
bool gameOver = false;
int board[3][3] = { 0 }; //  игровое поля: 0 = пусто, 1 = крестик, 2 = нолик
bool computerPlayer = false;

bool playerMoved = false;


void makingMove(HWND hwnd, int index);
void compMakingMove(HWND hwnd);

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    ::hInstance = hInstance;
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}



void checkWinner(HWND hwnd)
{
    // горизонтали и вертикали
    for (int i = 0; i < 3; ++i)
    {
        if (board[i][0] != 0 && board[i][0] == board[i][1] && board[i][0] == board[i][2])
        {
            gameOver = true;
            MessageBox(hwnd, (board[i][0] == 1) ? L"Крестики выиграли!" : L"Нолики выиграли!", L"Игра окончена", MB_OK);
            return;
        }

        if (board[0][i] != 0 && board[0][i] == board[1][i] && board[0][i] == board[2][i])
        {
            gameOver = true;
            MessageBox(hwnd, (board[0][i] == 1) ? L"Крестики выиграли!" : L"Нолики выиграли!", L"Игра окончена", MB_OK);
            return;
        }
    }

    //  диагонали
    if (board[0][0] != 0 && board[0][0] == board[1][1] && board[0][0] == board[2][2])
    {
        gameOver = true;
        MessageBox(hwnd, (board[0][0] == 1) ? L"Крестики выиграли!" : L"Нолики выиграли!", L"Игра окончена", MB_OK);
        return;
    }

    if (board[0][2] != 0 && board[0][2] == board[1][1] && board[0][2] == board[2][0])
    {
        gameOver = true;
        MessageBox(hwnd, (board[0][2] == 1) ? L"Крестики выиграли!" : L"Нолики выиграли!", L"Игра окончена", MB_OK);
        return;
    }

    //  ничья
    bool bDraw = true;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (board[i][j] == 0)
            {
                bDraw = false;
                break;
            }
        }
        if (!bDraw)
            break;
    }

    if (bDraw && !gameOver)
    {
        gameOver = true;
        MessageBox(hwnd, L"Ничья!", L"Игра окончена", MB_OK);
        return;
    }

}

void switchTurn()
{
    if (playerTurn)
    {
        playerTurn = false;
    }
    else
    {
        playerTurn = true;
    }

}



void makingMove(HWND hwnd, int index)
{
    int row = index / 3;
    int col = index % 3;

    if (board[row][col] == 0)
    {
        board[row][col] = playerTurn ? 1 : 2;
        HBITMAP hBitmap = playerTurn ? LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1)) : LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
        SendMessage(buttons[index], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
        switchTurn();
        checkWinner(hwnd);

       
        if (!playerTurn && !gameOver)
        {
            compMakingMove(hwnd);
        }
    }
}

void compMakingMove(HWND hwnd)
{
    if (gameOver) 
        return;

    int emptyCells[9]; //  индексs пустых ячеек
    int numEmptyCells = 0; // Кол-во пустых ячеек

    // Поиск пустіх ячек на игровом поле board
    for (int i = 0; i < 9; ++i)
    {
        int row = i / 3;
        int col = i % 3;
        if (board[row][col] == 0)
        {
            emptyCells[numEmptyCells++] = i;
        }
    }
    
    
    if (numEmptyCells == 0)
    {
        gameOver = true;
        MessageBox(hwnd, L"Ничья!", L"Игра окончена", MB_OK);
        return;
    }

  
    if (numEmptyCells > 0)
    {
        int randomIndex = rand() % numEmptyCells; 
        makingMove(hwnd, emptyCells[randomIndex]); 

      
        checkWinner(hwnd);
    }
}

void newGame(HWND hwnd)
{
   
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            board[i][j] = 0;
        }
    }

    // Сбросила кнопкт на форме
    for (int i = 0; i < 9; ++i)
    {
        SendMessage(buttons[i], BM_SETIMAGE, IMAGE_BITMAP, NULL);
        EnableWindow(buttons[i], TRUE);
    }

  
    playerTurn = true;
    gameOver = false;
    playerMoved = false;



    if (computerPlayer)
    {
      
        board[1][1] = 2; //  0 в центральную ячейку
        HBITMAP hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
        SendMessage(buttons[4], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
        playerTurn = true;
    }
    else
    {
         if (playerMoved) {
            compMakingMove(hwnd);
        }
    }
}


INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        for (int i = 0; i < 9; ++i)
        {
            buttons[i] = GetDlgItem(hwnd, IDC_BUTTON1 + i);
        }
        newGame(hwnd);
        return TRUE;
    }
    case WM_COMMAND:
    {
        if (LOWORD(wParam) >= IDC_BUTTON1 && LOWORD(wParam) <= IDC_BUTTON9 && HIWORD(wParam) == BN_CLICKED)
        {
            if (!gameOver)
            {
                int index = LOWORD(wParam) - IDC_BUTTON1;
                makingMove(hwnd, index);

              
                if (computerPlayer && !playerTurn && !gameOver)
                {
                    compMakingMove(hwnd);
                }
            }
            return TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON10 && HIWORD(wParam) == BN_CLICKED)
        {
         
            newGame(hwnd);
            return TRUE;
        }
        else if (LOWORD(wParam) == IDC_RADIO1 && HIWORD(wParam) == BN_CLICKED)
        {
           
            computerPlayer = false;
            newGame(hwnd); 
            return TRUE;
        }
        else if (LOWORD(wParam) == IDC_RADIO2 && HIWORD(wParam) == BN_CLICKED)
        {
           
            computerPlayer = true;
            newGame(hwnd); 
            return TRUE;
        }

        break;
    }


    case WM_CLOSE:
        EndDialog(hwnd, 0);
        return TRUE;
    }

    return FALSE;
}
