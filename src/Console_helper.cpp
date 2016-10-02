// functions used in the public Console methods are defined here

#include "Console.hpp"
#include "WinAPIException.hpp"
#include <Windows.h>
#include <sstream>
#include <cstdlib> // need realloc

using std::stringstream;

void winapiutil::Console::getHandle() {
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (handle == INVALID_HANDLE_VALUE) {
        throw WinAPIException("Failed to retrieve console handle");
    }
}

void winapiutil::Console::getCSBI() {
    if (!GetConsoleScreenBufferInfo(handle, &consoleScreenBuffer)) {
        stringstream error;
        error << "Failed to retrieve screen buffer info: Error code " << GetLastError();
        throw WinAPIException(error.str());
    }
}

void winapiutil::Console::setCSBSize(int x, int y) {
    COORD desiredSize = {x, y};
    if (!SetConsoleScreenBufferSize(handle, desiredSize)) {
        stringstream error;
        error << "Failed to resize console screen buffer: Error code " << GetLastError();
        throw WinAPIException(error.str());
    }
    CHAR_INFO* copy = offscreenBuffer;
    offscreenBuffer = (CHAR_INFO*)realloc(offscreenBuffer, x*y * sizeof *offscreenBuffer);
    if (offscreenBuffer == NULL) {
        free(copy); // copying came in handy!
        stringstream error;
        throw WinAPIException("Failed to reallocate memory to background screen buffer.");
    }
}

void winapiutil::Console::setCWSize(int x, int y) {
    SMALL_RECT r = {0}; // Zero-initialize
    r.Right = x; r.Bottom = y;
    if (!SetConsoleWindowInfo(handle, TRUE, &r)) {
        stringstream error;
        error << "Failed to set console window size: Error code " << GetLastError();
        throw WinAPIException(error.str());
    }
}

void winapiutil::Console::checkBounds(int x, int y) {
    // Check if resizing the window to this size (in characters)
    // will pose a problem
    stringstream error; // in case user specified invalid input

    if (x < 1 || y < 1) {
        error << "Invalid console size: " << x << ", " << y << "; x & y must be >= 1";
        throw WinAPIException(error.str());
    }

    if (x >= maxSize.X) {
        error << "Invalid console width: " << x << "; must be < " << maxSize.X;
        throw WinAPIException(error.str());
    }
    
    if (y >= maxSize.Y) {
        error << "Invalid console height: " << y << "; must be < " << maxSize.Y;
        throw WinAPIException(error.str());
    }
}

// NOTE: For reason why the below two methods work, see http://stackoverflow.com/a/12642749
int winapiutil::Console::getWindowWidth() {
    return consoleScreenBuffer.srWindow.Right - consoleScreenBuffer.srWindow.Left + 1;
}

int winapiutil::Console::getWindowHeight() {
    return consoleScreenBuffer.srWindow.Bottom - consoleScreenBuffer.srWindow.Top + 1;
}

void winapiutil::Console::getWindowPosition() {
    HWND consoleWindow = GetConsoleWindow();
    if (!GetWindowRect(consoleWindow, &windowPosition)) {
        stringstream error;
        error << "Failed to retrieve console window position: Error code " << GetLastError();
        throw WinAPIException(error.str());
    }
}