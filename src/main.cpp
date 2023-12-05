//
// Created by tqcq on 2023/12/5.
//

#include <string>
#include <ulib/log/log.h>
#include <string.h>

int main(int argc, char* argv[]) {
    ULOG_SET_STRIPPED_PREFIX_LEN(strlen(__FILE__) - strlen("src/main.cpp"));
    ULOG_TRACE("Hello, world!");
    return 0;
}
