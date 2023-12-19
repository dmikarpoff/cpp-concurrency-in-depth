// 2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <chrono>
#include <iostream>
#include <thread>

void clean()
{
  std::cout << "Start cleaning" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(3));
  std::cout << "Cleaning completed" << std::endl;
}

void speed_ahead()
{
  std::cout << "Speed ahead started" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "Speed ahead completed" << std::endl;
}

void stop()
{
  std::cout << "Stop started" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "Stop completed" << std::endl;
}

int main()
{
  for (;;)
  {
    int n{0};
    std::cin >> n;
    switch (n)
    {
    case 1:
    {
      std::thread t{ &clean };
      t.detach();
      break;
    }
    case 2:
    case 3:
    {
      std::thread t{ n == 2 ? &speed_ahead : &stop };
      t.join();
      break;
    }
    case 100:
      return 0;
    default:
      std::cout << "Invalid order" << std::endl;
      break;
    }
  }
}
