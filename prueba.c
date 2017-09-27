#include <stdio.h>

#define WINDOWS_PLATFORM 1
#define OTHER_PLATFORM 0

#ifdef _WIN32
#define PLATFORM 1
#else
#define PLATFORM 0
#endif

int main() {
  if(PLATFORM == WINDOWS_PLATFORM)printf("Windows\n");
  else printf("Other platform\n");
}
