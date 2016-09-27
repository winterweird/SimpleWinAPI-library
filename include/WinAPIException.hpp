#ifndef WINAPIEXCEPTION_HPP
#define WINAPIEXCEPTION_HPP

#include <exception>
#include <string>

// I don't wanna add an extra file just for this. It's fine as header-only.
namespace winapiutil {
    class WinAPIException : public std::exception {
        public:
            WinAPIException(std::string whatArg) : whatStr(whatArg) {}
            ~WinAPIException() throw() {}
            const char* what() const throw() { return whatStr.c_str(); }
        private:
            std::string whatStr;
    };
}

#endif /* WINAPIEXCEPTION_HPP */