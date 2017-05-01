//
// Created by Guy on 23/02/2017.
//

#include <Forest.h>
#include "../../headers/render/BufferedRenderer.h"
#include <Config.h>

#include <iostream>
#include <string>
#include <Utils.h>

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
            int attr = 0;
            if(Configuration::Instance().UseBlockRenderer())
                attr = utils::setConsoleColour(GetCellBackground(cell), GetCellForeground(cell));
            else attr = utils::setConsoleColour(GetCellForeground(cell), GetCellBackground(cell));
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
    SetConsoleTextAttribute(isAActive ? bufferA : bufferB, utils::setConsoleColour(WHITE, BLACK));
#if RENDER_DEBUG
    RenderDebug(forest);
#endif

}

void BufferedRenderer::RenderDebug(Forest *forest) {
    for(std::string line : GetDebugLines(forest)) {
        WriteConsole(isAActive ? bufferA : bufferB, line.c_str(), line.length(), NULL, NULL);
        WriteConsole(isAActive ? bufferA : bufferB, newLineStr.c_str(), newLineStr.length(), NULL, NULL);
    }
}

void BufferedRenderer::Dispose() {
    TextRenderer::Dispose();
    CloseHandle(bufferA);
    CloseHandle(bufferB);
}


int BufferedRenderer::GetCellForeground(Cell cell) {
    Tree* t = cell.tree;
#if USE_COLOURS
    if(!t->IsAlive()) {
        if(Configuration::Instance().UseBlockRenderer()) {
            if(cell.states->wall)
                return WALL_COLOUR;
            return DEFAULT_BACKGROUND;
        }
    }
    if(cell.states->wall)
        return WALL_COLOUR;

    if(t->IsIgnited() && !t->IsBurning())
        return IGNITED_TREE_COLOUR;
    if(t->IsBurning())
        return BURNING_TREE_COLOUR;
    if(t->IsAlive())
        return LIVING_TREE_COLOUR;
#endif

    if(Configuration::Instance().UseBlockRenderer()) {
        return DEFAULT_BACKGROUND;
    }
    return DEFAULT_TEXT_COLOUR;
}

int BufferedRenderer::GetCellBackground(Cell cell) {
#if USE_COLOURS
    if(cell.states->damp)
        return DAMP_GROUND_COLOUR;
    if(cell.states->dry)
        return DRY_GROUND_COLOUR;
    if(cell.states->wall)
        return WALL_COLOUR;
#endif
    if(Configuration::Instance().UseBlockRenderer())
        return DEFAULT_TEXT_COLOUR;
    return DEFAULT_BACKGROUND;
}