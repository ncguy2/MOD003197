//
// Created by Guy on 23/02/2017.
//

#include <Forest.h>
#include <render/TextRenderer.h>
#include <render/BufferedRenderer.h>

#if IS_WINDOWS

BufferedRenderer::BufferedRenderer() :
        bufferA(CreateConsoleScreenBuffer(GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL)),
        bufferB(CreateConsoleScreenBuffer(GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL)),
        isAActive(false) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    COORD size{columns, rows};

    SetConsoleScreenBufferSize(bufferA, size);
    SetConsoleScreenBufferSize(bufferB, size);
}

void BufferedRenderer::Render(Forest *forest) {
    isAActive = !isAActive;
    utils::clsAlt(isAActive ? bufferA : bufferB);

    std::string strBlock;
    int currentCol = -1;

    for(int x = 0; x < WORLD_SIZE_Y; x++) {
        strBlock.clear();
        for (int y = 0; y < WORLD_SIZE_X; y++) {
            Cell cell = forest->GetCell(y, x);
            int attr = utils::setConsoleColour(GetCellForeground(cell), GetCellBackground(cell));
            if(currentCol != attr) {
                WriteConsole(isAActive ? bufferA : bufferB, strBlock.c_str(), strBlock.length(), NULL, NULL);
                currentCol = attr;
                strBlock.clear();
                SetConsoleTextAttribute(isAActive ? bufferA : bufferB, (WORD) attr);
            }
            strBlock.append(renderer->RenderCell(cell));
        }
        WriteConsole(isAActive ? bufferA : bufferB, strBlock.c_str(), strBlock.length(), NULL, NULL);
        WriteConsole(isAActive ? bufferA : bufferB, newLineStr.c_str(), newLineStr.length(), NULL, NULL);
    }
    int err = SetConsoleActiveScreenBuffer(isAActive ? bufferA : bufferB);
    if(err == 0) {
        LPSTR messageBuffer;
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                       0, GetLastError(),
                       0, (LPSTR) &messageBuffer, 0, NULL);
        throw messageBuffer;
    }
}

void BufferedRenderer::RenderDebug(Forest *forest) {

}

void BufferedRenderer::Dispose() {
    TextRenderer::Dispose();
    CloseHandle(bufferA);
    CloseHandle(bufferB);
}


#else

BufferedRenderer::BufferedRenderer() {

}

void BufferedRenderer::Render(Forest *forest) {
    TextRenderer::Render(forest);
}

void BufferedRenderer::RenderDebug(Forest *forest) {
    TextRenderer::RenderDebug(forest);
}

#endif