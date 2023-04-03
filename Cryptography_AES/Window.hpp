#pragma once
#include "Framework.hpp"
#include <RichEdit.h>

#define MSFTEDIT_DLL_PATH "Msftedit.dll"

//#pragma comment(lib, MSFTEDIT_DLL_PATH) // - we do LoadLibrary now

namespace Window {

    // PREDEFINED FUNCTIONS
    ATOM                MyRegisterClass(HINSTANCE&, const wchar* const);
    BOOL                InitInstance(HINSTANCE&, const int&, const wchar* const,const wchar* const);
    LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

    // GLOBALS
    HINSTANCE currentProcess;

    block LoadRichEdit() {
        LoadLibrary(TEXT(MSFTEDIT_DLL_PATH));
    }

    // 1. Create single line richedit
    // 2. Create multiline with scrollbar
    // 3. Make it possible to extract value from such field.
    //  eg. to byte buffor and back to RichEdit

    HWND CreateRichEdit(
        HWND& parentWindow,
        const pair<int32>& position,
        const pair<int32>& area,
        HINSTANCE process,
        const wchar* preText = L"Type here"
    ) {
        HWND hwndEdit = CreateWindowEx(0, MSFTEDIT_CLASS, preText,
            ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP,
            position.x, position.y, area.x, area.y,
            parentWindow, NULL, process, NULL);

        return hwndEdit;
    }

    ATOM MyRegisterClass(
        HINSTANCE& process, 
        const wchar* const windowClassName
    ) {
        WNDCLASSEXW wcex { 0 };
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = process;
        wcex.hIcon = LoadIcon(process, MAKEINTRESOURCE(IDI_CRYPTOGRAPHYAES));
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = windowClassName;
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

        return RegisterClassExW(&wcex);
    }

    BOOL InitInstance(
        HINSTANCE& process,
        const int& nCmdShow, 
        const wchar* const windowClassName,
        const wchar* const windowTitle
    ) {
        currentProcess = process;

        HWND windowHandle = CreateWindowW(
            windowClassName, 
            windowTitle, 
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, 0, 
            CW_USEDEFAULT, 0, 
            nullptr, 
            nullptr, 
            process,
            nullptr
        );

        if (!windowHandle) return FALSE;

        const pair<int32> position { 0, 0 }, area { 100, 100};


        CreateRichEdit(windowHandle, position, area, process);

        ShowWindow(windowHandle, nCmdShow);
        UpdateWindow(windowHandle);

        return TRUE;
    }

    LRESULT CALLBACK WndProc(
        HWND hWnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    ) {
        switch (message) {
            case WM_COMMAND: {
                int wmId = LOWORD(wParam);
                switch (wmId) {
                    //case IDM_ABOUT:
                    //    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                    //    break;
                    case IDM_EXIT:
                        DestroyWindow(hWnd);
                        break;
                    default:
                        return DefWindowProc(hWnd, message, wParam, lParam);
                }
            } break;

            case WM_PAINT: {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hWnd, &ps);
                EndPaint(hWnd, &ps);
            } break;

            case WM_DESTROY:
                PostQuitMessage(0);
                break;

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
        }

        return 0;
    }
}