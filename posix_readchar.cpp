//
// Created by busby08 on 11/07/25.
//


#ifndef _win32

#include "posix_readchar.h"

#include <unistd.h>

#include "library.h"

extern "C" {
#include <termios.h>
}

template <typename T>
bool vector_contains(const std::vector<T> &vector, const T &element) {
    for (const auto &item : vector) {
        if (item == element)
            return true;
    }

    return false;
}


char posix_readchar::readchar() {
    struct termios old_settings, term;

    char ch;

    const int fd = fileno(stdin);
    tcgetattr(fd, &old_settings);
    term = old_settings;

    try {
        term.c_lflag &= ~(ICANON | ECHO | IGNBRK | BRKINT);
        tcsetattr(fd, TCSANOW, &term);

        read(fd, &ch, 1);

    } catch (...) {}

    tcsetattr(fd, TCSADRAIN, &old_settings);

    return ch;
}


std::string posix_readchar::readkey() {
    char c1 = readchar();

    if (vector_contains(readchar::INTERRUPT_KEYS, c1))
        throw readchar::exceptions::KeyboardInterrupt();

    if (c1 != '\x1B')
        return std::string() + c1;


    const char c2 = readchar();
    std::vector c2_block = {'\x4f', '\x5b'};
    if (!vector_contains(c2_block, c2))
        return std::string() + c1 + c2;

    const char c3 = readchar();
    const std::vector c3_block = {'\x31', '\x32', '\x33', '\x34', '\x35', '\x36'};
    if (!vector_contains(c3_block, c3))
        return std::string() + c1 + c2 + c3;

    const char c4 = readchar();
    const std::vector c4_block = {
        '\x31', '\x32', '\x33', '\x34', '\x35', '\x37', '\x38', '\x39'};
    if (!vector_contains(c4_block, c4))
        return std::string() + c1 + c2 + c3 + c4;

    const char c5 = readchar();
    return std::string() + c1 + c2 + c3 + c4 + c5;
}

#endif
