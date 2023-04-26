#include "Framework.hpp"
#include "Window.hpp"
#include "AES/main.hpp"

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

        { // TESTS

            //{ // Single Block
            //    const AES::Block nocoded {
            //        0x00, 0x11, 0x22, 0x33,
            //        0x44, 0x55, 0x66, 0x77,
            //        0x88, 0x99, 0xaa, 0xbb,
            //        0xcc, 0xdd, 0xee, 0xff
            //    };
            //    
            //    Tests::Test128(AES::TEST::Key128::sample1, nocoded);
            //    Tests::Test192(AES::TEST::Key192::sample1, nocoded);
            //    Tests::Test256(AES::TEST::Key256::sample1, nocoded);
            //}

            // No encoding decoding.
            //Tests::ReadWriteTest();

            { // FILE
                //const wchar* const nocodedFilePath = LR"(data/kryptoTest.pdf)";
                //const wchar* const encodedFilePath = LR"(data/aaa.pdf)";
                //const wchar* const decodedFilePath = LR"(data/bbb.pdf)";
                const wchar* const nocodedFilePath = LR"(data/1.txt)";
                const wchar* const encodedFilePath = LR"(data/2.txt)";
                const wchar* const decodedFilePath = LR"(data/3.txt)";
                uint8 extraBytes;
                //AES::ReadEncodeWrite<AES::Key128>(extraBytes, nocodedFilePath, encodedFilePath, AES::TEST::Key128::sample1);
                //AES::ReadDecodeWrite<AES::Key128>(encodedFilePath, decodedFilePath, AES::TEST::Key128::sample1, extraBytes);
                //AES::ReadEncodeWrite<AES::Key192>(extraBytes, nocodedFilePath, encodedFilePath, AES::TEST::Key192::sample1);
                //AES::ReadDecodeWrite<AES::Key192>(encodedFilePath, decodedFilePath, AES::TEST::Key192::sample1, extraBytes);
                AES::ReadEncodeWrite<AES::Key256>(extraBytes, nocodedFilePath, encodedFilePath, AES::TEST::Key256::sample1);
                AES::ReadDecodeWrite<AES::Key256>(encodedFilePath, decodedFilePath, AES::TEST::Key256::sample1, extraBytes);
            }
            
        }
        
    }

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}