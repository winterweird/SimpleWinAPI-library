#ifndef CURSORPOSITION_HPP
#define CURSORPOSITION_HPP

#include <Windows.h>

namespace winapiutil {
    class CursorPosition {
        public:
            CursorPosition(HANDLE);
            
            // Return CursorPosition for method chaining.
            // Not sure how useful it'd be, but I imagine it could
            // improve elegance occasionally.
            CursorPosition& getPosition();
            CursorPosition& moveTo(int, int);           // Jump to the absolute position x, y in the output buffer
            CursorPosition& moveTo_relative(int, int);  // Relative offset dx, dy
            CursorPosition& moveToOnScreen(int, int);   // Treats currently visible top-left as 0, 0
            
            CursorPosition& moveToLine(int);            // Doesn't change column
            CursorPosition& moveToLine_relative(int);   // ----------"----------
            CursorPosition& moveToColumn(int);          // Doesn't change line
            CursorPosition& moveToColumn_relative(int); // ---------"---------
            
            CursorPosition& nextLine(); // Repositions cursor on beginning of next line
            CursorPosition& prevLine(); // Repositions cursor on beginning of previous line

            int line();   // assumes getPosition has been called, i.e. x & y updated
            int column(); // ---------------------------"---------------------------

        private:
            HANDLE consoleHandle;
            int x;
            int y;
    };
}

#endif /* CURSORPOSITION_HPP */