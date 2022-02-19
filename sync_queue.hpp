#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

template<typename T>
class sync_queue
{
public:
    void push(T &&data)
    {
        std::lock_guard<std::mutex> guard(write_mutex);
        
        queue.push(std::forward<T>(data));
        not_empty.notify_one();
    }
    
    T pop()
    {
        std::unique_lock<std::mutex> guard(write_mutex);
        if(queue.empty())
        {
            not_empty.wait(guard);
        }
        
        T data = queue.front();
        queue.pop();
        return data;
    }
    
    sync_queue &operator<<(T &&data)
    {
        std::lock_guard<std::mutex> guard(write_mutex);
        
        queue.push(std::forward<T>(data));
        not_empty.notify_one();
        return *this;
    }
    
    sync_queue &operator>>(T &data)
    {
        std::unique_lock<std::mutex> guard(write_mutex);
        if(queue.empty())
        {
            not_empty.wait(guard);
        }
        
        data = queue.front();
        queue.pop();
        return *this;
    }
    
private:
    std::queue<T> queue;
    std::mutex write_mutex;
    std::condition_variable not_empty;
};
