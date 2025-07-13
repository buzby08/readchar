//
// Created by busby08 on 11/07/25.
//


#ifndef _win32

#include "posix_readchar.h"

#include <unistd.h>
#include <fcntl.h>

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


void posix_readchar::posix_readchar_helper::set_nonblocking(int fd, bool enable) {
    if (enable) {
        old_flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, old_flags | O_NONBLOCK);
        return;
    }

    fcntl(fd, F_SETFL, old_flags);
}

int posix_readchar::readcharNonBlocking() {
    struct termios old_settings, term;

    char ch;

    const int fd = fileno(stdin);
    tcgetattr(fd, &old_settings);
    term = old_settings;

    int result = 0;

    try {
        term.c_lflag &= ~(ICANON | ECHO | IGNBRK | BRKINT);
        tcsetattr(fd, TCSANOW, &term);

        posix_readchar_helper::set_nonblocking(fd, true);

        result = read(fd, &ch, 1);

    } catch (...) {}

    tcsetattr(fd, TCSADRAIN, &old_settings);
    posix_readchar_helper::set_nonblocking(fd, false);

    if (result > 0)
        return static_cast<unsigned char>(ch);

    return -1;
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

std::string posix_readchar::readkeyNonBlocking() {
    const int c1_as_int = readcharNonBlocking();

    if (c1_as_int == -1)
        return "";

    const char c1 = static_cast<char>(c1_as_int);

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
