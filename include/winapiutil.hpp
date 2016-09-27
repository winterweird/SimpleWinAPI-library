#ifndef WINAPIUTIL_HPP
#define WINAPIUTIL_HPP

#include "WinAPIException.hpp"
#include "Console.hpp"

namespace winapiutil {
    Console getConsole(); // Console extends Handleable, means it has a handle
}

#endif /* WINAPIUTIL_HPP */