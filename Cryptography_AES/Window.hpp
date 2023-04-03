#pragma once
#include "Framework.hpp"
#include <RichEdit.h>
#include "FileIO.hpp"

#define MSFTEDIT_DLL_PATH "Msftedit.dll" // a.k.a TextEdit 4.1
#define BUTTON_CLASS L"BUTTON"

//#pragma comment(lib, MSFTEDIT_DLL_PATH) // - we do LoadLibrary now

namespace Window {

    // PREDEFINED FUNCTIONS
    ATOM                MyRegisterClass(HINSTANCE&, const wchar* const);
    BOOL                InitInstance(HINSTANCE&, const int&, const wchar* const,const wchar* const);
    LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

    // GLOBALS
    HINSTANCE currentProcess;
    HWND buttonConfirmIO, buttonEncode, buttonDecode,
        reInputPath, reOutputPath, reInput, reOutput;

    block LoadRichEdit() { LoadLibrary(TEXT(MSFTEDIT_DLL_PATH)); }

    auto CreateGroupBox (
        HINSTANCE& process,
        HWND& parentWindow,
        const pair<int32>& position,
        const pair<int32>& area,
        const wchar* caption = L"Caption"
    ) {
        HWND groupBox = CreateWindowEx(
            NULL, BUTTON_CLASS,
            caption,
            WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
            position.x, position.y,
            area.x, area.y,
            parentWindow,
            nullptr,
            process,
            nullptr
        );
        return groupBox;
    }

    auto CreateButton (
        HINSTANCE& process,
        HWND& parentWindow,
        const pair<int32>& position,
        const pair<int32>& area,
        const wchar* buttonText = L"Click Me!"
    ) {
        HWND windowButton = CreateWindowEx(
            NULL, BUTTON_CLASS,
            buttonText,
            WS_CHILD | WS_VISIBLE,
            position.x, position.y,
            area.x, area.y,
            parentWindow,
            nullptr,
            process,
            nullptr
        );

        return windowButton;
    }

    auto CreateRichEdit (
        HINSTANCE& process,
        HWND& parentWindow,
        const pair<int32>& position,
        const pair<int32>& area,
        const uint32&  windowStyles,
        const wchar* preText = L"Type here"
    ) {

        HWND windowEdit = CreateWindowEx(
            NULL, MSFTEDIT_CLASS, 
            preText,
            windowStyles,
            position.x, position.y, 
            area.x, area.y,
            parentWindow, 
            nullptr,
            process, 
            nullptr
        );

        return windowEdit;
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
        const wchar* const windowTitle,
        const pair<int32>& windowPosition,
        const pair<int32>& windowArea
    ) {
        currentProcess = process;

        HWND windowHandle = CreateWindowW(
            windowClassName, 
            windowTitle, 
            WS_OVERLAPPEDWINDOW,
            windowPosition.x, windowPosition.y,
            windowArea.x, windowArea.y,
            nullptr, 
            nullptr, 
            process,
            nullptr
        );

        if (!windowHandle) return FALSE;

        { // Creation of inner windows.

            { // Caption Boxes

                // Different windows styles 7,8,10 and such have their own nonClientArea.
                //  for shoutcut this is how it should be on windows 10.
                const pair<int32> nonClientAreaOffset { 15, 28 };

                const pair<int32> 
                    positionWindowFile { 10, 0 },
                    areaWindowFile { windowArea.x - 20 - nonClientAreaOffset.x, 60 },
                    positionWindowText { 10, areaWindowFile.y },
                    areaWindowText { windowArea.x - 20 - nonClientAreaOffset.x, windowArea.y - 20 - nonClientAreaOffset.y - areaWindowFile.y};

                const wchar* captionRegionFile = L"File", *captionRegionText = L"Text";

                CreateGroupBox(process, windowHandle, positionWindowFile, areaWindowFile, captionRegionFile);
                CreateGroupBox(process, windowHandle, positionWindowText, areaWindowText, captionRegionText);

                { // File Windows

                    const pair<int32> 
                        positionInputFile { 20, 20 }, areaInputFile { 400, 24 + 4 },
                        positionOutputFile { 20 + areaInputFile.x + 10, 20 }, areaOutputFile { 400, 24 + 4 },
                        positionConfirm { areaOutputFile.x + positionOutputFile.x + 10, 20 }, areaConfirm { 100, 24 + 4 };

                    const uint32 singleLineStyle = WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP;

                    const wchar
                        *inputPreText = L"Input file path", 
                        *outputPreText = L"Output file path",
                        *confirmText = L"Confirm";

                    reInputPath = CreateRichEdit(process, windowHandle, positionInputFile, areaInputFile, singleLineStyle, inputPreText);
                    reOutputPath = CreateRichEdit(process, windowHandle, positionOutputFile, areaOutputFile, singleLineStyle, outputPreText);
                    buttonConfirmIO = CreateButton(process, windowHandle, positionConfirm, areaConfirm, confirmText);
                }

                { // Text Windows

                    const pair<int32>
                        positionInput { 20, positionWindowText.y + 20 },
                        areaInput { 400, areaWindowText.y - 20 - 10 },
                        positionOutput { positionInput.x + areaInput.x + 10, positionWindowText.y + 20 },
                        areaOutput { 400, areaWindowText.y - 20 - 10 },
                        positionEncode { positionOutput.x + areaOutput.x + 10, positionWindowText.y + 20 },
                        areaEncode { 100, 28 },
                        positionDecode { positionOutput.x + areaOutput.x + 10, positionWindowText.y + 20 + areaEncode.y + 10 },
                        areaDecode { 100, 28 };


                    const uint32 multiLineStyle = ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP;

                    const wchar
                        *inputPreText = L"Text",
                        *outputPreText = L"",
                        *encodeText = L"Encode",
                        *decodeText = L"Decode";

                    reInput = CreateRichEdit(process, windowHandle, positionInput, areaInput, multiLineStyle, inputPreText);
                    reOutput = CreateRichEdit(process, windowHandle, positionOutput, areaOutput, multiLineStyle, outputPreText);

                    // Show Scrollbar
                    SendMessageW(reInput, EM_SHOWSCROLLBAR, SB_VERT, TRUE);
                    SendMessageW(reOutput, EM_SHOWSCROLLBAR, SB_VERT, TRUE);

                    buttonEncode = CreateButton(process, windowHandle, positionEncode, areaEncode, encodeText);
                    buttonDecode = CreateButton(process, windowHandle, positionDecode, areaDecode, decodeText);
                }
            }

            

            const pair<int32> position { 20, 20 }, area { 100, 100};

            const uint32 singleLineStyle = ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP,
                multiLineStyle = WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP;
        }

        ShowWindow(windowHandle, nCmdShow);
        UpdateWindow(windowHandle);

        return TRUE;
    }

    block OnButtonConfirmClicked(
        const HWND& windowHandle
    ) {
        // READ LENGTH
        const size inputFilePathLength = SendMessageW(reInputPath, WM_GETTEXTLENGTH, NULL, NULL);
        const size inputStringTerminationPosition = inputFilePathLength + 1;
        const size outputFilePathLength = SendMessageW(reOutputPath, WM_GETTEXTLENGTH, NULL, NULL);
        const size outputStringTerminationPosition = outputFilePathLength + 1;

        wchar* inputPathBuffor = new wchar[inputStringTerminationPosition];
        wchar* outputPathBuffor = new wchar[outputStringTerminationPosition];

        // READ DATA // !!! check result ???
        SendMessageW(reInputPath, WM_GETTEXT, inputStringTerminationPosition, (LPARAM)inputPathBuffor);
        SendMessageW(reOutputPath, WM_GETTEXT, outputStringTerminationPosition, (LPARAM)outputPathBuffor);

        { // PROCESS
            std::vector<byte> readData(FileIO::Read::File(inputPathBuffor));
            FileIO::Write::File(outputPathBuffor, readData.size(), readData.data());
        }

        delete[] inputPathBuffor;
        delete[] outputPathBuffor;
    }

    block OnButtonEncodeClick(
        const HWND& windowHandle
    ) {

        // READ LENGTH
        const size inputLength = SendMessageW(reInput, WM_GETTEXTLENGTH, NULL, NULL);
        const size inputStringTerminationPosition = inputLength + 1;

        wchar* inputBuffor = new wchar[inputStringTerminationPosition];

        // READ DATA
        SendMessageW(reInput, WM_GETTEXT, inputStringTerminationPosition, (LPARAM)inputBuffor);

        { // PROCESS
            wchar* outputBuffer = inputBuffor;

            SendMessageW(reOutput, WM_SETTEXT, NULL, (LPARAM)outputBuffer);

            
        }

        delete[] inputBuffor;

        //MessageBox(windowHandle, inputBuffor, L"Nacisnąłeś przycisk!", MB_ICONINFORMATION);
    }

    block OnButtonDecodeClick(
        const HWND& windowHandle
    ) {
        // READ LENGTH
        const size inputLength = SendMessageW(reOutput, WM_GETTEXTLENGTH, NULL, NULL);
        const size inputStringTerminationPosition = inputLength + 1;

        wchar* inputBuffor = new wchar[inputStringTerminationPosition];

        // READ DATA
        SendMessageW(reOutput, WM_GETTEXT, inputStringTerminationPosition, (LPARAM)inputBuffor);

        { // PROCESS
            wchar* outputBuffer = inputBuffor;

            SendMessageW(reInput, WM_SETTEXT, NULL, (LPARAM)outputBuffer);


        }

        delete[] inputBuffor;
    }

    LRESULT CALLBACK WndProc(
        HWND windowHandle,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    ) {
        switch (message) {
            case WM_COMMAND: {

                int wmId = LOWORD(wParam);
                auto id = (HWND)lParam;

                if (id == buttonConfirmIO) {
                    OnButtonConfirmClicked(windowHandle);
                } else if (id == buttonEncode) {
                    OnButtonEncodeClick(windowHandle);
                } else if (id == buttonDecode) {
                    OnButtonDecodeClick(windowHandle);
                }

                switch (wmId) {
                    case IDM_EXIT:
                        DestroyWindow(windowHandle);
                        break;
                    default:
                        return DefWindowProc(windowHandle, message, wParam, lParam);
                }

            } break;

            case WM_GETTEXTLENGTH: {
                MessageBox(windowHandle, L"Nacisnąłeś przycisk!", L"2!", MB_ICONINFORMATION);
            } break;

            case WM_PAINT: {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(windowHandle, &ps);
                EndPaint(windowHandle, &ps);
            } break;

            case WM_DESTROY:
                PostQuitMessage(0);
                break;

            default:
                return DefWindowProc(windowHandle, message, wParam, lParam);
        }

        return 0;
    }
}