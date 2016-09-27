#ifndef HANDLEABLE_HPP
#define HANDLEABLE_HPP

#include <Windows.h> // I gave up making it light
namespace winapiutil {
    class Handleable {
        public:
            virtual void resize(int xsize, int ysize) = 0; // Absolute resizing
            virtual void resize_relative(int dx, int dy) = 0; // Relative resizing
            virtual void reposition(int xpos, int ypos) = 0; // Absolute repositioning
            virtual void reposition_relative(int dx, int dy) = 0; // Relative repositioning
            virtual void getHandle() = 0; // put handle in handle field
        protected:
            HANDLE handle; // Handle to this Handleable
    };
}

#endif /* HANDLEABLE_HPP */