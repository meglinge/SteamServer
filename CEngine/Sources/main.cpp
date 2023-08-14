#include <cstdio>
#include <spdlog/spdlog.h>
#include <mimalloc.h>
#include <GLFW/glfw3.h>
int main() {
     int i = 10;
     i++;
     spdlog::info("Welcome to spdlog!{}", i);
     return 0;
}
