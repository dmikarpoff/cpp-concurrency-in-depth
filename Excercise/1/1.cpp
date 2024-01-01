#include <iostream>
#include <thread>

void test()
{
    std::cout << "Hello from test\n";
}

void functionA()
{
    std::thread thread_c{&test};
    std::cout << "Hello from functionA\n";
    thread_c.join();
}

void functionB()
{
    std::cout << "Hello from functionB\n";
}

int main()
{
    std::thread thread_a{&functionA};
    std::thread thread_b{&functionB};
    std::cout << "Hello from main\n";
    thread_a.join();
    thread_b.join();
    return 0;
}
