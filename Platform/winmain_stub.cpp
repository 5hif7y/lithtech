#include "Platform/platform.h"
#include <cstdio>
extern "C" int WinMain(void*, void*, char*, int);
int main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    printf("LithTech demo stub main (graphify) -> calling WinMain\n");
    // If WinMain not linked (demo uses main), just return
    return 0;
}
