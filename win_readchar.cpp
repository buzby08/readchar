//
// Created by busby08 on 11/07/25.
//

#ifdef _WIN32

#include "win_readchar.h"

#include "utf8.h"

#include "library.h"


template <typename T>
bool vector_contains(const std::vector<T> &vector, const T &element) {
    for (const auto &item : vector) {
        if (item == element)
            return true;
    }

    return false;
}

int win_readchar::readcharNonBlocking() {
    if (_kbhit()) {
        return static_cast<unsigned char>(_getwch());
    }

    return -1;
}

char win_readchar::readchar() {
    return _getwch();
}

std::string win_readchar::readkeyNonBlocking() {
    const int ch_as_int = readcharNonBlocking();
    if (ch_as_int == -1)
        return "";

    const char ch = static_cast<char>(ch_as_int);

    std::string ch_as_string = std::string() + ch;

    if (vector_contains(readchar::INTERRUPT_KEYS, ch))
        throw readchar::exceptions::KeyboardInterrupt();

    const std::vector check_one = {'\x00', '\xe0'};
    if (vector_contains(check_one, ch)) {
        const int ch2_as_int = readcharNonBlocking();
        char ch2 = static_cast<char>(ch2_as_int);

        if (ch2_as_int == -1)
            throw readchar::exceptions::MissingCharacter();

        ch_as_string = std::string() + '\x00' + ch2;
    }

    const uint32_t codepoint = get_codepoint(ch_as_string);
    if (codepoint >= 0xD800 && codepoint <= 0xDFFF) {
        const int ch3_as_int = readcharNonBlocking();
        char ch3 = static_cast<char>(ch3_as_int);

        if (ch3_as_int == -1)
            throw readchar::exceptions::MissingCharacter();

        ch_as_string += ch3;
        ch_as_string = normalise_utf8(ch_as_string);
    }

    return ch_as_string;
}


std::string win_readchar::readkey() {
    const char ch = readchar();
    std::string ch_as_string = std::string() + ch;

    if (vector_contains(readchar::INTERRUPT_KEYS, ch))
        throw readchar::exceptions::KeyboardInterrupt();

    const std::vector check_one = {'\x00', '\xe0'};
    if (vector_contains(check_one, ch))
        ch_as_string = std::string() + '\x00' + readchar();

    const uint32_t codepoint = get_codepoint(ch_as_string);
    if (codepoint >= 0xD800 && codepoint <= 0xDFFF) {
        ch_as_string += readchar();
        ch_as_string = normalise_utf8(ch_as_string);
    }

    return ch_as_string;
}

uint32_t win_readchar::get_codepoint(const std::string &utf8_input) {
    auto it = utf8_input.begin();
    const uint32_t codepoint = utf8::next(it, utf8_input.end());
    return codepoint;
}


std::string win_readchar::normalise_utf8(const std::string &utf8_input) {
    const uint32_t codepoint = get_codepoint(utf8_input);
    std::string out;
    utf8::append(codepoint, std::back_inserter(out));
    return out;
}

#endif
