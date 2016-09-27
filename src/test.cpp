#include "winapiutil.hpp"
#include <iostream>
#include <string>

using std::cout; using std::endl; using std::cin;
using std::string;

int main() {
    winapiutil::Console console = winapiutil::getConsole();

    while (true) {
        string mode;
        while (!(mode == "resize" || mode == "move" || mode == "resize-rl"
                || mode == "move-rl" || mode == "exit")) {
            cout << "Enter mode (resize, move, resize-rl, move-rl) or 'exit' to exit: ";
            cin >> mode;
        }
        if (mode == "exit") break;
        int a, b;
        cout << "Enter an integer 'x': ";
        cin >> a;
        cout << "Enter an integer 'y': ";
        cin >> b;
        try {
            if (mode == "move") {
                console.reposition(a, b);
            }
            else if (mode == "move-rl") {
                console.reposition_relative(a, b);
            }
            else if (mode == "resize") {
                console.resize(a, b);
            }
            else {
                console.resize_relative(a, b);
            }
        } catch (winapiutil::WinAPIException& e) {
            cout << e.what() << endl;
        }
    }
    
}