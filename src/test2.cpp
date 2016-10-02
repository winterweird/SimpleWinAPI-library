#include <iostream>
#include <string> // for "Press Enter to continue" using getline()
#include "winapiutil.hpp"

using winapiutil::Console;
using std::cout; using std::cin; using std::endl;

int main() {
    Console console = winapiutil::getConsole();
    int line = 6;
    int column = 24;

    console.cursor.moveTo(line, column);
    cout << "At line 6, column 24!";

    console.cursor.moveTo(line+1, column+1);
    cout << "At line 7, column 25!";

    console.cursor.nextLine();
    cout << "Beginning of line 8";
    for (int i = 0; i < console.getWindowHeight(); i++) cout << "aaaaah!\n";

    console.cursor.moveToLine(255);
    cout << "Line 255!";
    console.cursor.moveToOnScreen(0, 0);
    cout << "Topleft corner of currently visible screen";
    console.cursor.moveToOnScreen(-5, -50);
    cout << "I don't even know, but hopefully it makes sense";

    console.cursor.moveTo(0, 0);
    cout << "Will clear currently visible area of screen.";
    console.cursor.nextLine(); // Like how I made this glorified '\n' character?
    cout << "Press Enter to continue";

    std::string throwawayString;
    std::getline(cin, throwawayString);

    console.clearVisibleScreen();

    // NOTE: TODO-list has (?) for features I'm not sure whether to/how to add
    //       and (<number>) for implementation priority. Features which have
    //       been added are marked with (X).

    // TODO: Make it easy to read from the console's output as well              (?)  (4)
    // TODO: Add support for text attributes                                          (1)
    // TODO: Create a simple, intuitive interface for filesystem interaction          (6)
    // TODO: Support for clearing the screen buffer                                   (X)
    //       - Support for clearing an area of the console
    //           - Clear specified line? Current line? Relative line? Clear
    //             everything from current pos? Everything on currently visible
    //             part of screen?
    // TODO: Movement on currently visible part of screen                             (X)
    // TODO: Support for shifting printed text left or right (current line only)      (5)
    //       - operator<< & operator>>                                           (?)
    //       NOTE: First I should make reading from the screen buffer easy
    // TODO: Support for soft-wrapping text                                      (?)  (2)
    // TODO: Message box wrapper                                                 (?)  (3)
    // TODO: Console screen double buffering FFS!                                     (X)
    //       TODO: Rename "readToOffscreenBuffer" to "syncOffscreenBuffer"       (?)
    //       TODO: Conditional overriding of the screen buffer                   (?)
    //             E.g. when offscreenBuffer.Char.Attributes != 0
    //             (This would require restructuring, which I'm not up to rn)
}