#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

class JoinableExecutor
{
 public:
  explicit JoinableExecutor(const std::atomic<bool>& done_flag) : done_flag_{ done_flag } {}
  ~JoinableExecutor()
  {
      worker_thread_.join();
  }

  template <class F>
  void Push(F&& f)
  {
      std::lock_guard lock{mutex_};
      tasks_.emplace(std::forward<F>(f));
  }

 private:
  void Run()
  {
    while (!done_flag_.load())
    {
        std::unique_lock lock{mutex_};
        if (tasks_.empty())
        {
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue;
        }
        auto cmd = tasks_.front();
        tasks_.pop();
        lock.unlock();
        cmd();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }

  std::queue<std::function<void()>> tasks_;
  std::mutex mutex_;
  const std::atomic<bool>& done_flag_;
  std::thread worker_thread_{ [this] {Run(); } };
};

class CleaningCrew : private JoinableExecutor
{
 public:
  using JoinableExecutor::JoinableExecutor;

  void Clean()
  {
    Push([] {std::cout << "Cleaning" << std::endl; });
  }
};

class EngineCrew : private JoinableExecutor
{
 public:
  using JoinableExecutor::JoinableExecutor;

  void SpeedAhead()
  {
      Push([] {std::cout << "Speed ahead" << std::endl; });
  }
  void Stop()
  {
      Push([] {std::cout << "Stop" << std::endl; });
  }
};

int main()
{
    std::atomic<bool> done_flag{false};
    CleaningCrew cleaning_crew{done_flag};
    EngineCrew engine_crew{done_flag};

    for (;;)
    {
        int n{ 0 };
        std::cin >> n;
        switch (n)
        {
        case 1:
            cleaning_crew.Clean();
            break;
        case 2:
            engine_crew.SpeedAhead();
            break;
        case 3:
            engine_crew.Stop();
            break;
        case 100:
            done_flag.store(true);
            return 0;
        default:
            std::cout << "Invalid order" << std::endl;
            break;
        }
    }
}

