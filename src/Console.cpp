#include "Console.hpp"
#include "WinAPIException.hpp"
#include "CursorPosition.hpp"
#include <Windows.h>
#include <algorithm> // std::max
#include <cstdlib> // I trust malloc

using std::stringstream;

winapiutil::Console::Console() : cursor(NULL) {
    getHandle();
    maxSize = GetLargestConsoleWindowSize(handle);

    // get a default for restoration later
    getCSBI();
    defaultConsoleScreenBuffer = consoleScreenBuffer;

    getWindowPosition();
    defaultWindowPosition = windowPosition;

    cursor = CursorPosition(handle);

    // Annoyed at how you need to explicitly cast return value of malloc
    // in C++ </rant>
    int sbSize = consoleScreenBuffer.dwSize.X * consoleScreenBuffer.dwSize.Y;
    offscreenBuffer = (CHAR_INFO*)std::malloc(sbSize * sizeof *offscreenBuffer);
}

winapiutil::Console::~Console() {
    // reset everything
    consoleScreenBuffer = defaultConsoleScreenBuffer;
    int columns = getWindowWidth();
    int rows = getWindowHeight();
    setCWSize(columns-1, rows-1);

    setCSBSize(defaultConsoleScreenBuffer.dwSize.X, defaultConsoleScreenBuffer.dwSize.Y);
    reposition(defaultWindowPosition.left, defaultWindowPosition.top);

    std::free(offscreenBuffer);
    // More will be added as I extend this library
}

void winapiutil::Console::resize(int x, int y) {
    // Override Handleable::resize
    // Do magic to make this work
    // Note: x and y is given in character width/height units
    checkBounds(x, y);

    // okay time to do magic

    getCSBI();

    // We must ensure that the screen buffer is big enough, because if not
    // then bad things happen D:
    int screenBufferWidth = std::max((int)consoleScreenBuffer.dwSize.X, x);
    int screenBufferHeight = std::max((int)consoleScreenBuffer.dwSize.Y, y);
    setCSBSize(screenBufferWidth, screenBufferHeight);

    setCWSize(x-1, y-1);
}

void winapiutil::Console::resize_relative(int dx, int dy) {
    // Override Handleable::resize_relative
    getCSBI();

    int actualX = dx + getWindowWidth();
    int actualY = dy + getWindowHeight();
    checkBounds(actualX, actualY);

    // ensure there is space
    int screenBufferWidth = std::max((int)consoleScreenBuffer.dwSize.X, actualX);
    int screenBufferHeight = std::max((int)consoleScreenBuffer.dwSize.Y, actualY);
    setCSBSize(screenBufferWidth, screenBufferHeight);

    setCWSize(actualX-1, actualY-1);
}

void winapiutil::Console::reposition(int x, int y) {
    // Override Handleable::reposition
    // Okay, to be fair, this was actually surprisingly simple. But still...
    // I'm making it simpler o3o
    HWND consoleWindow = GetConsoleWindow();
    SetWindowPos(consoleWindow, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void winapiutil::Console::reposition_relative(int dx, int dy) {
    // Override Handleable::reposition_relative
    getWindowPosition();
    reposition(windowPosition.left + dx, windowPosition.top + dy);
}