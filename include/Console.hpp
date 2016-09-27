#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "Handleable.hpp"
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
        private:
            CONSOLE_SCREEN_BUFFER_INFO defaultConsoleScreenBuffer;
            CONSOLE_SCREEN_BUFFER_INFO consoleScreenBuffer;
            RECT defaultWindowPosition;
            RECT windowPosition;
            COORD maxSize;

            int getWindowWidth();
            int getWindowHeight();
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