#ifndef UTILS_H
#define UTILS_H
/* MACRO DEFInitIONS */
#define COL_BLUE "\033[0;34m"
#define COL_RED "\033[0;31m"
#define COL_GREEN "\033[0;32m"
#define COL_YELLOW "\033[0;33m"
#define COL_MAGENTA "\033[0;35m"
#define COL_DEFAULT "\033[0m"

#define UTILS_ASSERT(condition, msg)                               \
    if (!(condition))                                              \
    {                                                              \
        std::cerr << COL_RED << (msg) << COL_DEFAULT << std::endl; \
        exit(EXIT_FAILURE);                                        \
    }

#define COUT std::cout
#define CERR std::cerr
#define ENDL std::endl
#endif /*  UTILS_H */