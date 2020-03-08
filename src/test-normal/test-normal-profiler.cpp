#include <google/profiler.h> 

void test1() {
    int i = 0;
    while (i < 1000) i++;
}

void test2() {
    int i = 0;
    while (i < 2000) i++;
}

void test3() {
    for (int i = 0; i < 100000; ++i){
        test1();
        test2();
    }
}

int main() {
     ProfilerStart("test-normal.prof"); // test-normal.prof is the name of profile file
    test3();
     ProfilerStop();
    return 0;
}
