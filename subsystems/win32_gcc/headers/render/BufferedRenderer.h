//
// Created by Guy on 23/02/2017.
//

#ifndef FIRESIM_BUFFEREDRENDERER_H
#define FIRESIM_BUFFEREDRENDERER_H

#include <render/TextRenderer.h>

class BufferedRenderer : public TextRenderer {
public:
    BufferedRenderer();

    void Render(Forest *forest) override;
    void RenderDebug(Forest *forest) override;

    void Dispose() override;

    /**
     * Gets the colour of the cell foreground
     * @param cell
     * @return The integer value of the foreground colour to use
     */
    int GetCellForeground(Cell cell);
    int GetCellBackground(Cell cell);

protected:
    std::string newLineStr = "\n";
    bool isAActive;

    HANDLE bufferA;
    HANDLE bufferB;
};

#endif //FIRESIM_BUFFEREDRENDERER_H
