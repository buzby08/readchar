//
// Created by busby08 on 11/07/25.
//

#ifndef POSIX_READCHAR_H
#define POSIX_READCHAR_H

#ifndef _WIN32

#include <string>
#include <vector>

namespace posix_readchar {
    char readchar();

    std::string readkey();
}

#endif //_WIN32 ELSE

#endif //POSIX_READCHAR_H
