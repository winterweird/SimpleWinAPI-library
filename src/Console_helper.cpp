// functions used in the public Console methods are defined here

#include "Console.hpp"
#include "WinAPIException.hpp"
#include <Windows.h>
#include <sstream>

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
}

void winapiutil::Console::setCWSize(int x, int y) {
    SMALL_RECT r = {0};
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

int winapiutil::Console::getWindowWidth() {
    return consoleScreenBuffer.srWindow.Right - consoleScreenBuffer.srWindow.Left;
}

int winapiutil::Console::getWindowHeight() {
    return consoleScreenBuffer.srWindow.Bottom - consoleScreenBuffer.srWindow.Top;
}

void winapiutil::Console::getWindowPosition() {
    HWND consoleWindow = GetConsoleWindow();
    if (!GetWindowRect(consoleWindow, &windowPosition)) {
        stringstream error;
        error << "Failed to retrieve console window position: Error code " << GetLastError();
        throw WinAPIException(error.str());
    }
}