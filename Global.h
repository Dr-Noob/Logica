#ifndef __Global__
#define __Global__

#define BOOLEAN_FALSE         0
#define BOOLEAN_TRUE          1

#ifdef _WIN32

#define RED
#define GREEN
#define RESET

#else

#define RED "\x1b[31;1m"
#define GREEN "\x1b[32;1m"
#define RESET "\x1b[0m"

#endif


#endif
