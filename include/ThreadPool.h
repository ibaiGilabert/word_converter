#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <condition_variable>
#include <functional>
#include <future>
#include <vector>
#include <thread>
#include <queue>

class ThreadPool
{
public:
    using Task = std::function<void()>;

    explicit ThreadPool(unsigned int numThreads) :
        mStopping(false)
    {
        Start(numThreads);
    }

    ~ThreadPool()
    {
        Stop();
    }

    template<class T>
    auto Enqueue(T task)->std::future<decltype(task())>
    {
        auto wrapper = std::make_shared<std::packaged_task<decltype(task()) ()>>(std::move(task));
        {
            std::unique_lock<std::mutex> lock{mEventMutex};
            mTasks.emplace([=] {
                (*wrapper)();
            });
        }
        mEventVar.notify_one();
        return wrapper->get_future();
    }

private:
    void Start(unsigned int  numThreads)
    {
        for (unsigned int i = 0; i < numThreads; ++i)
        {
            mThreads.emplace_back([=] {
                while (true)
                {
                    Task task;
                    {
                        std::unique_lock<std::mutex> lock{mEventMutex};
                        mEventVar.wait(lock, [=] {
                            return mStopping || !mTasks.empty();
                        });

                        if (mStopping && mTasks.empty()) {
                            break;
                        }
                        task = std::move(mTasks.front());
                        mTasks.pop();
                    }
                    task();
                }
            });
        }
    }

    void Stop() noexcept
    {
        {
            std::unique_lock<std::mutex> lock{mEventMutex};
            mStopping = true;
        }
        mEventVar.notify_all();

        for (auto &thread : mThreads)
        {
            thread.join();
        }
    }

private:
    std::vector<std::thread> mThreads;
    std::queue<Task> mTasks;

    std::condition_variable mEventVar;
    std::mutex mEventMutex;
    bool mStopping;
};

#endif // THREADPOOL_H
