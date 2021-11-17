#include "user_space_system_calls.h"

DEFN_SYSCALL1(print_string, 0, const char*);
DEFN_SYSCALL1(print_hex, 1, const char*);
DEFN_SYSCALL1(print_binary, 2, const char*);
