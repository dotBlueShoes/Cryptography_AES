﻿#include "Framework.hpp"
#include "Window.hpp"
#include "main.hpp"

// Globals
const wchar classNameMainWindow[] = L"MainWindow",
    titleMainWindow[] = L"AES 242422 242539";

int32 APIENTRY wWinMain (
    IN          HINSTANCE process,
    IN OPTIONAL HINSTANCE parentProcess,
    IN          LPWSTR    lpCmdLine,
    IN          int32     nCmdShow
) {

    UNREFERENCED_PARAMETER(parentProcess);
    UNREFERENCED_PARAMETER(lpCmdLine);

    { // Dynamic Library Loading.
        Window::LoadRichEdit();
    }

    { // Window Creation.
        const pair<int32> windowPosition { CW_USEDEFAULT, 0 }, windowArea { 1000, 500 };

        Window::MyRegisterClass(process, classNameMainWindow);
        if (!Window::InitInstance(process, nCmdShow, classNameMainWindow, titleMainWindow, windowPosition, windowArea)) return FALSE;

        //Tests::ReadWriteTest();
        Tests::KeyExpensionTest();
    }
    

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}