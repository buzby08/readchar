//
// Created by busby08 on 11/07/25.
//

#include "win_readchar.h"

#include <cstdint>

#include "utf8.h"
#include "library.h"

#ifdef _WIN32

template <typename T>
bool vector_contains(const std::vector<T> &vector, const T &element) {
    for (const auto &item : vector) {
        if (item == element)
            return true;
    }

    return false;
}

int win_readchar::readchar() {
    if (_kbhit()) {
        return static_cast<unsigned char>(_getwch()); // Does not echo
    } else {
        return -1; // Nothing pressed
    }
}


std::string win_readchar::readkey() {
    const int ch_as_int = readchar();
    if (ch_as_int == -1)
        return ""

    const char ch = static_cast<char>(ch_int)
    std::string ch_as_string = std::string() + ch;

    if (vector_contains(readchar::INTERRUPT_KEYS, ch))
        throw readchar::exceptions::KeyboardInterrupt();

    const std::vector check_one = {'\x00', '\xe0'};
    if (vector_contains(check_one, ch))
        ch_as_string = std::string() + '\x00' + readchar();

    if ("\uD800" <= ch_as_string && ch_as_string <= "\uDFFF") {
        ch_as_string += readchar();
        ch_as_string = normalise_utf8(ch_as_string);
    }

    return ch_as_string;
}

std::string win_readchar::normalise_utf8(std::string utf8_input) {
    auto it = utf8_input.begin();
    const uint32_t codepoint = utf8::next(it, utf8_input.end());
    std::string out;
    utf8::append(codepoint, std::back_inserter(out));
    return out;
}

#endif
