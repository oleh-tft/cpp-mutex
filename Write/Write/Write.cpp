#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <iostream>
#include <fstream>

using namespace std;

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

HWND hwnd;

DWORD WINAPI WriteToFiles(LPVOID lp)
{
    int amount = (int)lp;
    TCHAR buf[30];

    HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, false, TEXT("{9509D0D4-3552-4E02-B278-6A6E8F97206B}"));
    DWORD dwAnswer = WaitForSingleObject(hMutex, INFINITE);
    if (dwAnswer == WAIT_OBJECT_0)
    {
        for (int i = 0; i < amount; i++)
        {
            ifstream in(TEXT("source.txt"));
            wsprintf(buf, TEXT("source_copy_%d.txt"), i);
            ofstream out(buf);

            out << in.rdbuf(); //copy

            in.close();
            out.close();
        }
        ReleaseMutex(hMutex);
    }
    EndDialog(hwnd, 0);

    return 0;
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        hwnd = hWnd;
        CreateThread(NULL, 0, WriteToFiles, (LPVOID)5, 0, NULL);

        return TRUE;
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        return TRUE;
    }
    return FALSE;
}