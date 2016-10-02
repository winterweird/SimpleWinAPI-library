#include "Console.hpp"
#include "WinAPIException.hpp"
#include <sstream> // for error string building

// Methods handling clearing of the screen buffer go here
using std::string; // probably useful

void winapiutil::Console::writeToOffscreenBuffer(char ch) {
    // assumes updated CSBI
    // Fill the entire screen buffer with character ch
    writeToOffscreenBuffer(ch, 0, 0, consoleScreenBuffer.dwSize.Y, consoleScreenBuffer.dwSize.X);
}
void winapiutil::Console::writeToOffscreenBuffer(char ch, int ln, int col) {
    // assumes updated CSBI
    // Print character ch to the screen buffer at ln, col (absolute position)
}
void winapiutil::Console::writeToOffscreenBuffer(char ch, int startLn, int startCol, int nOfLines, int nOfCols) {
    // assumes updated CSBI
    // Fill the area of the screen buffer specified by top left
    // (startLn, startCol) position and bottom right
    // (startLn + nOfLines, startCol + nOfCols) coordinates

    int csbWidth = consoleScreenBuffer.dwSize.X;
    for (int l = 0; l < nOfLines; l++) {
        for (int c = 0; c < nOfCols; c++) {
            offscreenBuffer[startLn*csbWidth + startCol + l*csbWidth + c].Char.AsciiChar = ch;
        }
    }
}

void winapiutil::Console::readToOffscreenBuffer() {
    CHAR_INFO* currentBufferPosition = offscreenBuffer;

    int sbWidth = consoleScreenBuffer.dwSize.X;
    int sbHeight = consoleScreenBuffer.dwSize.Y;

    int charsRCOShouldBeAbleToHandle = 8000; // see explanation below
    int linesPerIteration = charsRCOShouldBeAbleToHandle/sbWidth;

    COORD bufferSize = {sbWidth, linesPerIteration};
    COORD startFrom = {0, 0};
    SMALL_RECT region = {0, 0, sbWidth, sbHeight};

    BOOL success; // just in case

    for (int i = 0; sbWidth * (sbHeight - i*linesPerIteration) > charsRCOShouldBeAbleToHandle; i++) {
        // The 8000 is an arbitrary limit, chosen based on the fact that on
        // my PC the ReadConsoleOutput function succeeds at an 80x100 size
        // but fails at an 80x200 one. Note that there is still no guarantee
        // the function  won't fail on any other system or in specific use
        // cases, because the WinAPI is a fucking piece of shit.

        // Bottom line, we need to sequentially read areas of the screen
        // into offscreenBuffer to keep ReadConsoleOutput from complaining.
        // This is achieved by, while the remaining area to read is larger
        // than 8000 characters, reading a certain amount of lines to the
        // buffer so that the amount of characters read is always < 8000.
        // This will hopefully keep RCO from making a fuss. In the end, we
        // read all the remaining characters onto the end of the buffer.

        success = ReadConsoleOutput(handle, currentBufferPosition, bufferSize, startFrom, &region);
        if (!success) {
            std::stringstream error;
            error << "Failed to read from screen buffer: Error code " << GetLastError();
            throw WinAPIException(error.str());
        }

        // Let's pretend the buffer starts somewhere it doesn't:
        currentBufferPosition += linesPerIteration*sbWidth;

        // Region affected can (and probably will) be changed by RCO. Undo this.
        // Also modify Top to be the current position where it should read from.
        region.Left  = 0;         region.Top    = (i+1)*linesPerIteration;
        region.Right = sbWidth;   region.Bottom = sbHeight;
    }

    // read the remaining crap
    bufferSize.Y = sbHeight - startFrom.Y;
    success = ReadConsoleOutput(handle, currentBufferPosition, bufferSize, startFrom, &region);
    if (!success) {
        std::stringstream error;
        error << "Failed to read from screen buffer: Error code " << GetLastError();
        throw WinAPIException(error.str());
    }
}

void winapiutil::Console::swapScreenBuffer() {
    // assumes updated CSBI
    COORD bufferSize = {consoleScreenBuffer.dwSize.X, consoleScreenBuffer.dwSize.Y};
    COORD startFrom = {0, 0};
    SMALL_RECT region = {0, 0, consoleScreenBuffer.dwSize.X, consoleScreenBuffer.dwSize.Y};
    
    WriteConsoleOutput(handle, offscreenBuffer, bufferSize, startFrom, &region);

    // read back into the offscreen buffer
    readToOffscreenBuffer();
}

void winapiutil::Console::clearScreen() {
    getCSBI();
    writeToOffscreenBuffer(' ');
    swapScreenBuffer();
}
void winapiutil::Console::clearVisibleScreen() {
    int topLn, leftCol;

    getCSBI();
    readToOffscreenBuffer(); // we don't want to erase anything not onscreen

    topLn = consoleScreenBuffer.srWindow.Top;
    leftCol = 0;

    writeToOffscreenBuffer(' ', topLn, leftCol, getWindowHeight(), getWindowWidth());
    swapScreenBuffer();
}

void winapiutil::Console::clearCurrentLine() {
    clearLine_relative(0);
}
void winapiutil::Console::clearLine(int n) {
    getCSBI();
    readToOffscreenBuffer(); // save current output
    // params: charToInsert, startLine, startCol, nOfLines, nOfCols
    writeToOffscreenBuffer(' ', n, 0, 1, getWindowWidth());
    swapScreenBuffer();
}
void winapiutil::Console::clearLine_relative(int n) {
    clearLine(cursor.line() + n);
}

void winapiutil::Console::clearLines(int n) {
    // start from current line
    getCSBI();
    readToOffscreenBuffer(); // save current output
    
    // params: charToInsert, startLine, startCol, nOfLines, nOfCols
    writeToOffscreenBuffer(' ', cursor.line(), 0, n, getWindowWidth());
    swapScreenBuffer();
}
void winapiutil::Console::clearCharacters(int n) {
    getCSBI();
    readToOffscreenBuffer(); // save current output
    writeToOffscreenBuffer(' ', cursor.line(), cursor.column(), 1, n);
    swapScreenBuffer();
}
void winapiutil::Console::clearPrintedCharacters(int n) {
    // Currently not a huge priority
    // When this gets implemented, it will go through the screen buffer
    // and clear away n printed characters, ignoring whitespace.
    throw WinAPIException("Not implemented. Sorry.");
}