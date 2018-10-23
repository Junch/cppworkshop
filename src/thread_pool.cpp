#include <future>
#include <gtest/gtest.h>
#include <queue>
#include <thread>

namespace thread_pool
{

///////////////////////////////////////////////////////////////////////////////
// No. 1
// Title: 基于C++11的线程池(threadpool),简洁且可以带任意多的参数
// Web: https://www.cnblogs.com/lzpong/p/6397997.html
//      https://github.com/lzpong/threadpool
///////////////////////////////////////////////////////////////////////////////

class ThreadPool
{
    using Task = std::function<void()>;
    std::vector<std::thread> pool_;
    std::queue<Task> tasks_;
    std::mutex task_lock_;
    std::condition_variable task_cv_;
    std::atomic<bool> stoped_;
    std::atomic<int> idlThrNum_;

  public:
    inline ThreadPool(unsigned short size = 4) : stoped_{false}
    {
        idlThrNum_ = size < 1 ? 1 : size;
        for (size = 0; size < idlThrNum_; ++size)
        {
            pool_.emplace_back([this] {
                while (!this->stoped_)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock{task_lock_};
                        task_cv_.wait(lock, [this] {
                            return (stoped_.load() || !tasks_.empty());
                        }); // wait till there is task or stoped
                        if (stoped_ && tasks_.empty())
                        {
                            return;
                        }
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    --idlThrNum_;
                    task();
                    ++idlThrNum_;
                }
            });
        }
    }

    inline ~ThreadPool()
    {
        stoped_.store(true);
        task_cv_.notify_all();
        for (std::thread &thread : pool_)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
    }

  public:
    template <class F, class... Args>
    auto commit(F &&f, Args &&... args) -> std::future<decltype(f(args...))>
    {
        if (stoped_.load())
        {
            throw std::runtime_error("commit on ThreadPool is stopped");
        }

        using RetType = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<RetType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<RetType> future = task->get_future();
        {
            std::lock_guard<std::mutex> lock{task_lock_};
            tasks_.emplace([task]() { (*task)(); });
        }
        task_cv_.notify_one();
        return future;
    }

    int idlCount() { return idlThrNum_; }
};

std::mutex s_cout;

int countdown(int from, int to)
{
    for (int i = from; i != to; --i)
    {
        {
            std::lock_guard<std::mutex> lock{s_cout};
            std::cout << std::hex << std::this_thread::get_id() << ": " << std::dec << i << '\n';
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    {
        std::lock_guard<std::mutex> lock{s_cout};
        std::cout << std::hex << std::this_thread::get_id() << ": "
                  << "Finished!\n";
    }
    return from - to;
}

TEST(threadpool, simple)
{
    ThreadPool executor{2};
    std::future<int> f5 = executor.commit(countdown, 5, 0);
    std::future<int> f10 = executor.commit(countdown, 10, 5);
    executor.commit(countdown, 15, 10);

    int r5 = f5.get();

    {
        std::lock_guard<std::mutex> lock{s_cout};
        std::cout << r5 << '\n';
    }
}

TEST(threadpool, idlCount)
{
    ThreadPool executor{};
    ASSERT_EQ(4, executor.idlCount());
}

} // namespace thread_pool