#pragma once
#include "Framework.hpp"
#include "Windows/MainTab.hpp"
#include "AES/main.hpp"
#include "FileIO.hpp"

#include <RichEdit.h>

#define MSFTEDIT_DLL_PATH "Msftedit.dll" // a.k.a TextEdit 4.1
#define BUTTON_CLASS L"BUTTON"

//#pragma comment(lib, MSFTEDIT_DLL_PATH) // - we do LoadLibrary now

//const AES::Key128 testKey {
//    0x00, 0x00, 0x00, 0x00,
//    0x00, 0x00, 0x00, 0x00,
//    0x00, 0x09, 0x00, 0x00,
//    0x00, 0x00, 0x00, 0x0f
//};

//const AES::Key128 testKey {
//    0x00, 0x01, 0x02, 0x03,
//    0x04, 0x05, 0x06, 0x07,
//    0x08, 0x09, 0x0a, 0x0b,
//    0x0c, 0x0d, 0x0e, 0x0f
//};

namespace Window {

    // PREDEFINED FUNCTIONS
    ATOM                MyRegisterClass(HINSTANCE&, const wchar* const);
    BOOL                InitInstance(HINSTANCE&, const int&, const wchar* const,const wchar* const);
    LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

    // GLOBALS
    HINSTANCE currentProcess;
    HWND buttonEncodePath, buttonDecodePath, buttonEncode, buttonDecode,
        reKey, reInputPath, reOutputPath, reInput, reOutput;

    uint8 aesBytesLeftPath, aesWordsLeft;

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

            { // Tab Window

                const pair<int32> tabPosition { 0, 0 };
                const pair<int32> tabStyleOffset { 16, 26 }; // this should be related to nonClientAreaOffset and tabXOffset... fix it later...
                const pair<int32> tabArea { 
                    windowArea.x - tabStyleOffset.x + 2 /* to hide bottom line */, 
                    windowArea.y - tabStyleOffset.y - 13 + 2 /* to hide bottom line */ 
                };
                const int32 tabXOffset = 24;

                Windows::MainTab::Create (
                    Windows::MainTab::tabHandle,
                    windowHandle,
                    process,
                    tabPosition,
                    tabArea
                );

                { // Caption Boxes
                
                    // Different windows styles 7,8,10 and such have their own nonClientArea.
                    //  for shoutcut this is how it should be on windows 10.
                    const pair<int32> nonClientAreaOffset { 15, 28 };
                    const pair<int32>
                        keyPosition { 20 + 40 + 10, 10 + tabXOffset },
                        keyArea { 870 + 37, 24 };
                
                    { // Key Value

                        const uint32 singleLineStyle = WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP;

                        const wchar
                            * preText = L"Key";

                        /* Text creation */
                        CreateWindow(L"STATIC", L"Key :", WS_VISIBLE | WS_CHILD | SS_LEFT, 20, 10 + tabXOffset + 3, 100, 100, windowHandle, nullptr, process, nullptr);
                        reKey = CreateRichEdit(process, windowHandle, keyPosition, keyArea, singleLineStyle, preText);
                    }

                    const pair<int32>
                        positionWindowFile { 10, keyPosition.y + keyArea.y + 10 },
                        areaWindowFile { 
                            windowArea.x - 20 - nonClientAreaOffset.x, 
                            60 + 60 + 24 
                        },
                        positionWindowText { 10, positionWindowFile.y + areaWindowFile.y },
                        areaWindowText {
                            windowArea.x - 20 - nonClientAreaOffset.x,
                            windowArea.y - 20 - nonClientAreaOffset.y - areaWindowFile.y - positionWindowFile.y
                    };

                    const wchar* captionRegionFile = L"File", * captionRegionText = L"Text";

                    CreateGroupBox(process, windowHandle, positionWindowFile, areaWindowFile, captionRegionFile);
                    CreateGroupBox(process, windowHandle, positionWindowText, areaWindowText, captionRegionText);

                    { // File Windows
                
                        const pair<int32>
                            positionInputFile { 20, positionWindowFile.y + 20 },
                            areaInputFile { 810, 24 + 10 + 18 },
                            positionOutputFile { 20, positionInputFile.y + areaInputFile.y + 10 },
                            areaOutputFile { 810, 24 + 10 + 18 },
                            positionEncode { areaOutputFile.x + positionOutputFile.x + 10, positionInputFile.y },
                            areaEncode { 100, 24 + 4 },
                            positionDecode { areaOutputFile.x + positionOutputFile.x + 10, positionEncode.y + areaEncode.y + 10 },
                            areaDecode { 100, 24 + 4 };
                
                        const uint32 singleLineStyle = WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP;
                
                        const wchar
                            * inputPreText = L"Input file path",
                            * outputPreText = L"Output file path",
                            * encodeText = L"Encode",
                            * decodeText = L"Decode";
                
                        reInputPath = CreateRichEdit(process, windowHandle, positionInputFile, areaInputFile, singleLineStyle, inputPreText);
                        reOutputPath = CreateRichEdit(process, windowHandle, positionOutputFile, areaOutputFile, singleLineStyle, outputPreText);

                        // Show Horizontal Scrollbar
                        SendMessageW(reInputPath, EM_SHOWSCROLLBAR, SB_HORZ, TRUE);
                        SendMessageW(reOutputPath, EM_SHOWSCROLLBAR, SB_HORZ, TRUE);

                        buttonEncodePath = CreateButton(process, windowHandle, positionEncode, areaEncode, encodeText);
                        buttonDecodePath = CreateButton(process, windowHandle, positionDecode, areaDecode, decodeText);
                    }
                
                    { // Text Windows
                
                        const pair<int32>
                            positionInput { 20, positionWindowText.y + 20 },
                            areaInput { 400, areaWindowText.y - 20 - 10  },
                            positionOutput { positionInput.x + areaInput.x + 10, positionInput.y },
                            areaOutput { 400, areaWindowText.y - 20 - 10 },
                            positionEncode { positionOutput.x + areaOutput.x + 10, positionWindowText.y + 20 },
                            areaEncode { 100, 28 },
                            positionDecode { positionOutput.x + areaOutput.x + 10, positionWindowText.y + 20 + areaEncode.y + 10 },
                            areaDecode { 100, 28 };
                
                
                        const uint32 multiLineStyle = ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP;
                
                        const wchar
                            * inputPreText = L"Text",
                            * outputPreText = L"",
                            * encodeText = L"Encode",
                            * decodeText = L"Decode";
                
                        reInput = CreateRichEdit(process, windowHandle, positionInput, areaInput, multiLineStyle, inputPreText);
                        reOutput = CreateRichEdit(process, windowHandle, positionOutput, areaOutput, multiLineStyle, outputPreText);
                
                        // Show Vertical Scrollbar
                        SendMessageW(reInput, EM_SHOWSCROLLBAR, SB_VERT, TRUE);
                        SendMessageW(reOutput, EM_SHOWSCROLLBAR, SB_VERT, TRUE);
                
                        buttonEncode = CreateButton(process, windowHandle, positionEncode, areaEncode, encodeText);
                        buttonDecode = CreateButton(process, windowHandle, positionDecode, areaDecode, decodeText);
                    }
                }
            }

            //const pair<int32> position { 20, 20 }, area { 100, 100};
            //
            //const uint32 singleLineStyle = ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP,
            //    multiLineStyle = WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP;
        }

        ShowWindow(windowHandle, nCmdShow);
        UpdateWindow(windowHandle);

        return TRUE;
    }

    block OnButtonEncodePathClicked(
        const HWND& windowHandle
    ) {

        // READ INPUT_FIELD LENGTH
        const size inputFilePathLength = SendMessageW(reInputPath, WM_GETTEXTLENGTH, NULL, NULL);
        const size inputStringTerminationPosition = inputFilePathLength + 1;
        const size outputFilePathLength = SendMessageW(reOutputPath, WM_GETTEXTLENGTH, NULL, NULL);
        const size outputStringTerminationPosition = outputFilePathLength + 1;
        const size keyValueLength = SendMessageW(reKey, WM_GETTEXTLENGTH, NULL, NULL);
        const size keyStringTerminationPosition = keyValueLength + 1;

        wchar* inputPathBuffor = new wchar[inputStringTerminationPosition];
        wchar* outputPathBuffor = new wchar[outputStringTerminationPosition];
        wchar* keyBuffor = new wchar[keyStringTerminationPosition];

        // READ INPUT_FIELD DATA
        SendMessageW(reInputPath, WM_GETTEXT, inputStringTerminationPosition, (LPARAM)inputPathBuffor);
        SendMessageW(reOutputPath, WM_GETTEXT, outputStringTerminationPosition, (LPARAM)outputPathBuffor);
        SendMessageW(reKey, WM_GETTEXT, keyStringTerminationPosition, (LPARAM)keyBuffor);

        { // PROCESS
            switch (Windows::MainTab::tabState) {

                default:
                case Windows::MainTab::AES_128: {
                    AES::Key128 key { 0 };
                    AES::WcharsToKey(key, keyBuffor, keyValueLength);

                    if (keyValueLength > 8) { // eg. 16 bytes
                        MessageBox(nullptr, L"Key value is to long! Max 8 signs for Key128!", LOG_TYPE, MB_OK);
                        return;
                    }

                    AES::ReadEncodeWrite<AES::Key128>(aesBytesLeftPath, inputPathBuffor, outputPathBuffor, key);
                    MessageBox(nullptr, L"Succefully Encrypted [128]", LOG_TYPE, MB_OK);
                } break;

                case Windows::MainTab::AES_192: {
                    AES::Key192 key { 0 };
                    AES::WcharsToKey(key, keyBuffor, keyValueLength);

                    if (keyValueLength > 12) { // eg. 24 bytes
                        MessageBox(nullptr, L"Key value is to long! Max 12 signs for Key192!", LOG_TYPE, MB_OK);
                        return;
                    }

                    AES::ReadEncodeWrite<AES::Key192>(aesBytesLeftPath, inputPathBuffor, outputPathBuffor, key);
                    MessageBox(nullptr, L"Succefully Encrypted [192]", LOG_TYPE, MB_OK);
                } break;

                case Windows::MainTab::AES_256: {
                    AES::Key256 key { 0 };
                    AES::WcharsToKey(key, keyBuffor, keyValueLength);

                    if (keyValueLength > 16) { // eg. 32 bytes
                        MessageBox(nullptr, L"Key value is to long! Max 16 signs for Key256!", LOG_TYPE, MB_OK);
                        return;
                    }

                    AES::ReadEncodeWrite<AES::Key256>(aesBytesLeftPath, inputPathBuffor, outputPathBuffor, key);
                    MessageBox(nullptr, L"Succefully Encrypted [256]", LOG_TYPE, MB_OK);
                }

            }
        }

        delete[] inputPathBuffor;
        delete[] outputPathBuffor;
        delete[] keyBuffor;
    }

    block OnButtonDecodePathClicked(
        const HWND& windowHandle
    ) {

        // READ INPUT_FIELD LENGTH
        const size inputFilePathLength = SendMessageW(reInputPath, WM_GETTEXTLENGTH, NULL, NULL);
        const size inputStringTerminationPosition = inputFilePathLength + 1;
        const size outputFilePathLength = SendMessageW(reOutputPath, WM_GETTEXTLENGTH, NULL, NULL);
        const size outputStringTerminationPosition = outputFilePathLength + 1;
        const size keyValueLength = SendMessageW(reKey, WM_GETTEXTLENGTH, NULL, NULL);
        const size keyStringTerminationPosition = keyValueLength + 1;

        wchar* inputPathBuffor = new wchar[inputStringTerminationPosition];
        wchar* outputPathBuffor = new wchar[outputStringTerminationPosition];
        wchar* keyBuffor = new wchar[keyStringTerminationPosition];

        // READ INPUT_FIELD DATA
        SendMessageW(reInputPath, WM_GETTEXT, inputStringTerminationPosition, (LPARAM)inputPathBuffor);
        SendMessageW(reOutputPath, WM_GETTEXT, outputStringTerminationPosition, (LPARAM)outputPathBuffor);
        SendMessageW(reKey, WM_GETTEXT, keyStringTerminationPosition, (LPARAM)keyBuffor);

        { // PROCESS
            switch (Windows::MainTab::tabState) {

                default:
                case Windows::MainTab::AES_128: {
                    AES::Key128 key { 0 };
                    AES::WcharsToKey(key, keyBuffor, keyValueLength);

                    if (keyValueLength > 8) { // eg. 16 bytes
                        MessageBox(nullptr, L"Key value is to long! Max 8 signs for Key128!", LOG_TYPE, MB_OK);
                        return;
                    }

                    AES::ReadDecodeWrite<AES::Key128>(inputPathBuffor, outputPathBuffor, key, aesBytesLeftPath);
                    MessageBox(nullptr, L"Succefully Decrypted [128]", LOG_TYPE, MB_OK);
                } break;

                case Windows::MainTab::AES_192: {
                    AES::Key192 key { 0 };
                    AES::WcharsToKey(key, keyBuffor, keyValueLength);

                    if (keyValueLength > 12) { // eg. 24 bytes
                        MessageBox(nullptr, L"Key value is to long! Max 12 signs for Key192!", LOG_TYPE, MB_OK);
                        return;
                    }

                    AES::ReadDecodeWrite<AES::Key192>(inputPathBuffor, outputPathBuffor, key, aesBytesLeftPath);
                    MessageBox(nullptr, L"Succefully Decrypted [192]", LOG_TYPE, MB_OK);
                } break;

                case Windows::MainTab::AES_256: {
                    AES::Key256 key { 0 };
                    AES::WcharsToKey(key, keyBuffor, keyValueLength);

                    if (keyValueLength > 16) { // eg. 32 bytes
                        MessageBox(nullptr, L"Key value is to long! Max 16 signs for Key256!", LOG_TYPE, MB_OK);
                        return;
                    }

                    AES::ReadDecodeWrite<AES::Key256>(inputPathBuffor, outputPathBuffor, key, aesBytesLeftPath);
                    MessageBox(nullptr, L"Succefully Decrypted [256]", LOG_TYPE, MB_OK);
                }

            }
        }

        delete[] inputPathBuffor;
        delete[] outputPathBuffor;
        delete[] keyBuffor;
    }

    block OnButtonEncodeClick(
        const HWND& windowHandle
    ) {

        // READ LENGTH
        const size inputLength = SendMessageW(reInput, WM_GETTEXTLENGTH, NULL, NULL);
        const size inputStringTerminationPosition = inputLength + 1;
        const size keyValueLength = SendMessageW(reKey, WM_GETTEXTLENGTH, NULL, NULL);
        const size keyStringTerminationPosition = keyValueLength + 1;

        wchar* inputBuffor = new wchar[inputStringTerminationPosition];
        wchar* keyBuffor = new wchar[keyStringTerminationPosition];

        // OUTPUT BUFFOR NECESSITIES
        const uint64 inputBytesLength = inputLength * 2;
        const uint64 blocksCount = inputBytesLength / 16;
        uint64 outputCount;

        if (inputBytesLength % 16 == 0)
            outputCount = (blocksCount * 8);
        else
            outputCount = (blocksCount * 8) + 8;
        
        wchar* outputBuffor = new wchar[outputCount + 1 /* null-termination*/];

        // READ DATA
        SendMessageW(reInput, WM_GETTEXT, inputStringTerminationPosition, (LPARAM)inputBuffor);
        SendMessageW(reKey, WM_GETTEXT, keyStringTerminationPosition, (LPARAM)keyBuffor);

        { // PROCESS
            switch (Windows::MainTab::tabState) {

                default:
                case Windows::MainTab::AES_128: {
                    AES::Key128 key { 0 };
                    AES::WcharsToKey(key, keyBuffor, keyValueLength);

                    if (keyValueLength > 8) { // eg. 16 bytes
                        MessageBox(nullptr, L"Key value is to long! Max 8 signs for Key128!", LOG_TYPE, MB_OK);
                        return;
                    }

                    AES::ReadEncodeWrite<AES::Key128>(aesWordsLeft, outputBuffor, inputBuffor, inputBytesLength, key);
                    MessageBox(nullptr, L"Succefully Encrypted [128]", LOG_TYPE, MB_OK);
                } break;

                case Windows::MainTab::AES_192: {
                    AES::Key192 key { 0 };
                    AES::WcharsToKey(key, keyBuffor, keyValueLength);

                    if (keyValueLength > 12) { // eg. 24 bytes
                        MessageBox(nullptr, L"Key value is to long! Max 12 signs for Key192!", LOG_TYPE, MB_OK);
                        return;
                    }

                    AES::ReadEncodeWrite<AES::Key192>(aesWordsLeft, outputBuffor, inputBuffor, inputBytesLength, key);
                    MessageBox(nullptr, L"Succefully Encrypted [192]", LOG_TYPE, MB_OK);
                } break;

                case Windows::MainTab::AES_256: {
                    AES::Key256 key { 0 };

                    if (keyValueLength > 16) { // eg. 32 bytes
                        MessageBox(nullptr, L"Key value is to long! Max 16 signs for Key256!", LOG_TYPE, MB_OK);
                        return;
                    }

                    AES::WcharsToKey(key, keyBuffor, keyValueLength);
                    AES::ReadEncodeWrite<AES::Key256>(aesWordsLeft, outputBuffor, inputBuffor, inputBytesLength, key);
                    MessageBox(nullptr, L"Succefully Encrypted [256]", LOG_TYPE, MB_OK);
                }

            }

            outputBuffor[outputCount] = L'\0';
            SendMessageW(reOutput, WM_SETTEXT, NULL, (LPARAM)outputBuffor);
        }

        delete[] inputBuffor;
        delete[] outputBuffor;
    }

    block OnButtonDecodeClick(
        const HWND& windowHandle
    ) {
        // READ LENGTH
        const size inputLength = SendMessageW(reOutput, WM_GETTEXTLENGTH, NULL, NULL);
        const size inputStringTerminationPosition = inputLength + 1;
        const size keyValueLength = SendMessageW(reKey, WM_GETTEXTLENGTH, NULL, NULL);
        const size keyStringTerminationPosition = keyValueLength + 1;

        wchar* inputBuffor = new wchar[inputStringTerminationPosition];
        wchar* keyBuffor = new wchar[keyStringTerminationPosition];

        // OUTPUT BUFFOR NECESSITIES
        const uint64 inputBytesLength = inputLength * 2;
        const uint64 blocksCount = inputBytesLength / 16; 
        
        uint64 outputCount;

        if (aesWordsLeft == 0)
            outputCount = (blocksCount * 8);
        else
            outputCount = (blocksCount * 8) - (8 - aesWordsLeft);

        wchar* outputBuffor = new wchar[outputCount + 1 /* null-termination*/];

        // READ DATA
        SendMessageW(reOutput, WM_GETTEXT, inputStringTerminationPosition, (LPARAM)inputBuffor);
        SendMessageW(reKey, WM_GETTEXT, keyStringTerminationPosition, (LPARAM)keyBuffor);

        { // PROCESS
            switch (Windows::MainTab::tabState) {

                default:
                case Windows::MainTab::AES_128: {
                    AES::Key128 key { 0 };
                    AES::WcharsToKey(key, keyBuffor, keyValueLength);

                    if (keyValueLength > 8) { // eg. 16 bytes
                        MessageBox(nullptr, L"Key value is to long! Max 8 signs for Key128!", LOG_TYPE, MB_OK);
                        return;
                    }

                    AES::ReadDecodeWrite<AES::Key128>(outputBuffor, inputBuffor, inputBytesLength, key, aesWordsLeft);
                    MessageBox(nullptr, L"Succefully Decrypted [128]", LOG_TYPE, MB_OK);
                } break;

                case Windows::MainTab::AES_192: {
                    AES::Key192 key { 0 };
                    AES::WcharsToKey(key, keyBuffor, keyValueLength);

                    if (keyValueLength > 12) { // eg. 24 bytes
                        MessageBox(nullptr, L"Key value is to long! Max 12 signs for Key192!", LOG_TYPE, MB_OK);
                        return;
                    }

                    AES::ReadDecodeWrite<AES::Key192>(outputBuffor, inputBuffor, inputBytesLength, key, aesWordsLeft);
                    MessageBox(nullptr, L"Succefully Decrypted [192]", LOG_TYPE, MB_OK);
                } break;

                case Windows::MainTab::AES_256: {
                    AES::Key256 key { 0 };
                    AES::WcharsToKey(key, keyBuffor, keyValueLength);

                    if (keyValueLength > 16) { // eg. 32 bytes
                        MessageBox(nullptr, L"Key value is to long! Max 16 signs for Key256!", LOG_TYPE, MB_OK);
                        return;
                    }

                    AES::ReadDecodeWrite<AES::Key256>(outputBuffor, inputBuffor, inputBytesLength, key, aesWordsLeft);
                    MessageBox(nullptr, L"Succefully Decrypted [256]", LOG_TYPE, MB_OK);
                }

            }

            outputBuffor[outputCount] = L'\0';
            SendMessageW(reInput, WM_SETTEXT, NULL, (LPARAM)outputBuffor);
        }

        delete[] inputBuffor;
        delete[] outputBuffor;
        delete[] keyBuffor;
    }

    LRESULT CALLBACK WndProc(
        HWND windowHandle,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    ) {
        switch (message) {

            case WM_NOTIFY: {
                LPNMHDR notyfication((LPNMHDR)lParam);
                if (notyfication->code == TCN_SELCHANGE) {

                    /// Indeks aktualnej kontrolki, Get Current Selected
                    const int index(SendMessage(Windows::MainTab::tabHandle, TCM_GETCURSEL, 0, 0)); 

                    switch (index) {

                        default:
                        case Windows::MainTab::ID_TAB_0: {
                            //MessageBox(nullptr, L"A", L"DEBUG", MB_OK);
                            Windows::MainTab::tabState = 0;
                        } break;

                        case Windows::MainTab::ID_TAB_1: {
                            //MessageBox(nullptr, L"B", L"DEBUG", MB_OK);
                            Windows::MainTab::tabState = 1;
                        } break;

                        case Windows::MainTab::ID_TAB_2: {
                            //MessageBox(nullptr, L"C", L"DEBUG", MB_OK);
                            Windows::MainTab::tabState = 2;
                        }

                    }
                }
            }

            case WM_COMMAND: {

                int wmId = LOWORD(wParam);
                auto id = (HWND)lParam;

                if (id == buttonEncodePath) {
                    OnButtonEncodePathClicked(windowHandle);
                } else if (id == buttonDecodePath) {
                    OnButtonDecodePathClicked(windowHandle);
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
                MessageBox(windowHandle, L"Nacisn¹³eœ przycisk!", L"2!", MB_ICONINFORMATION);
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