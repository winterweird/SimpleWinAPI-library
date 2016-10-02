#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "Handleable.hpp"
#include "CursorPosition.hpp"
#include <sstream>

namespace winapiutil {
    class Console : public Handleable {
        public:
            Console(); // constructor
            ~Console(); // destructor
            // all methods override Handlable virtual methods
            void resize(int, int);
            void resize_relative(int, int);
            void reposition(int, int);
            void reposition_relative(int, int);

            // These don't (but probably should)
            int getWindowWidth();
            int getWindowHeight();

            // These don't (and probably shouldn't)
            void clearScreen(); // clears out entire screen buffer
            void clearVisibleScreen(); // clears only currently visible screen buffer
            void clearLine(int n); // absolute position in screen buffer
            void clearLine_relative(int n); // relative offset from current line
            void clearLines(int n); // from current line
            void clearCurrentLine();
            void clearCharacters(int n); // remove n chars relative from current pos
            void clearPrintedCharacters(int n); // clear non-whitespace chars (wraps to next line)

            void writeToOffscreenBuffer(char);
            void writeToOffscreenBuffer(char, int, int);
            void writeToOffscreenBuffer(char, int, int, int, int);
            void readToOffscreenBuffer();
            void swapScreenBuffer();

            CursorPosition cursor; // handles cursor movement; see CursorPosition.hpp/-.cpp
        private:
            CONSOLE_SCREEN_BUFFER_INFO defaultConsoleScreenBuffer;
            CONSOLE_SCREEN_BUFFER_INFO consoleScreenBuffer;
            RECT defaultWindowPosition;
            RECT windowPosition;
            COORD maxSize;
            CHAR_INFO* offscreenBuffer;
            
            // the following methods throw exceptions on failure
            void getHandle();
            void getCSBI();
            void setCSBSize(int, int);
            void setCWSize(int, int);
            void getWindowPosition();
            void checkBounds(int, int);
    };
}

#endif /* CONSOLE_HPP */