# SimpleWinAPI-library
A simplified interface to the Windows API for C++

>Why?

There are a lot of things in the Windows API for C/C++ which are a hassle to deal with. The other day, a friend mentioned that I could resize a console window to suit my purposes in [a simple console game I made](https://github.com/winterweird/lol). This turned out to be more of a hassle to achieve than it should, in my opinion. So I decided to make a library interfacing the WinAPI so that simple things could be made simple.

This is a work in progress. I intend to build upon it, extending it further as I go on to create things utilizing other aspects of the WinAPI. I put it on GitHub in case it could be of use to someone else, but I cannot guarantee that it will work on your system even though it works on mine.

## Usage

```
#include "winapiutil.hpp"
...
winapiutil::Console console = winapiutil::getConsole();
// Do stuff with console; See src/test.cpp and src/test2.cpp for sample usage
// or check out Console.hpp and CursorPosition.hpp for an overview of the
// possibilities
```

(Currently, this library only supports operations on the console. These operations include: Cursor movement, console resizing/repositioning and clearing the screen buffer.)

In order to use this library, you will have to link it with your executable, either by creating a library file which you specify to your linker, or by adding all the source files to the compile command when compiling your program.

## Build instructions

As indicated in the makefile, you can compile each source file and archive them into a static library which you can then link with your project. These source files are, in order of decreasing dependency:

* src/winapiutil.cpp
* src/Console.cpp
* src/Console_clear.cpp
* src/Console_helper.cpp
* src/CursorPosition.cpp

I don't know how other compilers than the GNU compilers deal with stuff, so if there's anything which differs from your compiler, you'll have to figure that out on your own. Good luck!
