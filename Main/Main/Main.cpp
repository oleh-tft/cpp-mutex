#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <string>

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

HWND hFrom, hTo, hCopy, hStart;
int amount = 0;

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        hFrom = GetDlgItem(hWnd, IDC_EDIT1);
        hTo = GetDlgItem(hWnd, IDC_EDIT2);
        hCopy = GetDlgItem(hWnd, IDC_EDIT3);
        hStart = GetDlgItem(hWnd, IDC_BUTTON1);
        SetWindowText(hFrom, TEXT("source.txt"));
        SetWindowText(hTo, TEXT("result.txt"));

        return TRUE;
    case WM_COMMAND:
        if (wParam == IDC_BUTTON1)
        {
            int length = SendMessage(hCopy, WM_GETTEXTLENGTH, 0, 0);
            TCHAR* buffer = new TCHAR[length + 1];
            GetWindowText(hCopy, buffer, length + 1);
            amount = std::stoi(buffer);

            delete[] buffer;

            CreateMutex(0, FALSE, TEXT("{9509D0D4-3552-4E02-B278-6A6E8F97206B}"));
            STARTUPINFO st = { sizeof(st) };
            PROCESS_INFORMATION pr;
            TCHAR filename[20];
            wsprintf(filename, TEXT("%s"), TEXT("Write.exe"));
            if (!CreateProcess(NULL, filename, NULL, NULL, 0, 0, NULL, NULL, &st, &pr))
            {
                return FALSE;
            }
            CloseHandle(pr.hThread);
            CloseHandle(pr.hProcess);
            ZeroMemory(&st, sizeof(st));
            st.cb = sizeof(st);
            wsprintf(filename, TEXT("%s"), TEXT("Read.exe"));
            if (!CreateProcess(NULL, filename, NULL, NULL, 0, 0, NULL, NULL, &st, &pr))
            {
                return FALSE;
            }
            CloseHandle(pr.hThread);
            CloseHandle(pr.hProcess);
        }
        return TRUE;
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        return TRUE;
    }
    return FALSE;
}