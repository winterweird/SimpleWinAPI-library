#include "CursorPosition.hpp"

winapiutil::CursorPosition::CursorPosition(HANDLE h) {
    consoleHandle = h;
    getPosition();
}

winapiutil::CursorPosition& winapiutil::CursorPosition::getPosition() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(consoleHandle, &csbi);

    x = csbi.dwCursorPosition.X;
    y = csbi.dwCursorPosition.Y;
    
    return *this;
}

winapiutil::CursorPosition& winapiutil::CursorPosition::moveTo(int ln, int col) {
    COORD position = {col, ln};
    SetConsoleCursorPosition(consoleHandle, position);
    
    return getPosition(); // update the registered position
}
winapiutil::CursorPosition& winapiutil::CursorPosition::moveTo_relative(int ln, int col) {
    return moveTo(y+ln, x+col);
}
winapiutil::CursorPosition& winapiutil::CursorPosition::moveToOnScreen(int ln, int col) {
    // Aside from getPosition and moveTo, the only method with even a modicum
    // of complexity involved...
    // NOTE: This function includes an out-of-bounds check which "wraps around"
    // so that if there are for example 20 lines, then moveToOnScreen(20, 0) is
    // the same as moveToOnScreen(0, 0), and moveToOnScreen(-1, 0) is the same
    // as moveToOnScreen(19, 0, and so on and so forth).

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(consoleHandle, &csbi);

    int ln0 = csbi.srWindow.Top;
    int col0 = csbi.srWindow.Left;

    // We need to add one so that we get the "number of lines/columns", instead
    // of onscreen-coordinates of the topmost and leftmost line/column
    int height = csbi.srWindow.Bottom - ln0 + 1;
    int width = csbi.srWindow.Right - col0 + 1;

    // Praise be onto the order of operations!
    return moveTo(ln0 + (ln % height + height) % height, col0 + (col % width + width) % width);
}
winapiutil::CursorPosition& winapiutil::CursorPosition::moveToLine(int ln) {
    return moveTo(ln, x);
}
winapiutil::CursorPosition& winapiutil::CursorPosition::moveToLine_relative(int ln) {
    return moveTo_relative(ln, 0);
}
winapiutil::CursorPosition& winapiutil::CursorPosition::moveToColumn(int col) {
    return moveTo(y, col);
}
winapiutil::CursorPosition& winapiutil::CursorPosition::moveToColumn_relative(int col) {
    return moveTo_relative(0, col);
}

winapiutil::CursorPosition& winapiutil::CursorPosition::nextLine() {
    return moveTo(y + 1, 0);
}
winapiutil::CursorPosition& winapiutil::CursorPosition::prevLine() {
    return moveTo(y - 1, 0);
}

int winapiutil::CursorPosition::line() {
    return y;
}

int winapiutil::CursorPosition::column() {
    return x;
}