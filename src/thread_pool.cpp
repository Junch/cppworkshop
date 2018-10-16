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

class threadpool
{
    using Task = std::function<void()>;
    std::vector<std::thread> pool;
    std::queue<Task> tasks;
    std::mutex m_lock;
    std::condition_variable cv_task;
    std::atomic<bool> stoped;
    std::atomic<int> idlThrNum;

  public:
    inline threadpool(unsigned short size = 4) : stoped{false}
    {
        idlThrNum = size < 1 ? 1 : size;
        for (size = 0; size < idlThrNum; ++size)
        {
            pool.emplace_back([this] {
                while (!this->stoped)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock{this->m_lock};
                        this->cv_task.wait(lock, [this] {
                            return (this->stoped.load() || !this->tasks.empty());
                        }); // wait till there is task or stoped
                        if (this->stoped && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    idlThrNum--;
                    task();
                    idlThrNum++;
                }
            });
        }
    }

    inline ~threadpool()
    {
        stoped.store(true);
        cv_task.notify_all();
        for (std::thread &thread : pool)
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
        if (stoped.load())
            throw std::runtime_error("commit on ThreadPool is stopped");

        using RetType = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<RetType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<RetType> future = task->get_future();
        {
            std::lock_guard<std::mutex> lock{m_lock};
            tasks.emplace([task]() { (*task)(); });
        }
        cv_task.notify_one();
        return future;
    }

    int idlCount() { return idlThrNum; }
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
        std::cout << std::hex << std::this_thread::get_id() << ": " << "Finished!\n";
    }
    return from - to;
}

TEST(threadpool, simple)
{
    threadpool executor{2};
    std::future<int> f5 = executor.commit(countdown, 5, 0);
    std::future<int> f10 = executor.commit(countdown, 10, 5);
    executor.commit(countdown, 15, 10);

    int r5 = f5.get();

    {
        std::lock_guard<std::mutex> lock{s_cout};
        std::cout << r5 << '\n';
    }
}

} // namespace thread_pool