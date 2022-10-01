
#ifndef CONCURR_QUEUE_H
#define CONCURR_QUEUE_H

#include <mutex>
#include <condition_variable>
#include <queue>

namespace tapdev
{

    template <typename T>
    class concurrent_queue
    {
    public:
        void push_back(T &&data);

        void push_back(const T &data);

        bool empty() const;

        bool try_pop(T &popped);

        void wait_and_pop(T &popped);

    private:
        template <typename... Args>
        void emplace_back(Args &&...args);

        mutable std::mutex _queue_mutex{};
        mutable std::condition_variable _cond_var{};
        std::queue<T> _queue{};
    };

    template <typename T>
    void concurrent_queue<T>::push_back(T &&data)
    {
        this->emplace_back(std::move(data));
    }

    template <typename T>
    void concurrent_queue<T>::push_back(const T &data)
    {
        this->emplace_back(data);
    }

    template <typename T>
    bool concurrent_queue<T>::empty() const
    {
        std::lock_guard<std::mutex> lockg{this->_queue_mutex};
        return this->_queue.empty();
    }

    template <typename T>
    bool concurrent_queue<T>::try_pop(T &popped)
    {
        std::unique_lock<std::mutex> lockq{this->_queue_mutex};
        if (this->_queue.empty())
            return false;

        try
        {
            popped = std::move(this->_queue.front());
            this->_queue.pop_front();
        }
        catch (...)
        {
            return false;
        }

        return true;
    }

    template <typename T>
    void concurrent_queue<T>::wait_and_pop(T &popped)
    {
        std::unique_lock<std::mutex> lockq{this->_queue_mutex};
        while (this->_queue.empty())
            this->_cond_var.wait(lockq);

        try
        {
            popped = std::move(this->_queue.front());
            this->_queue.pop();
        }
        catch (...)
        {
        }
    }

    template <typename T>
    template <typename... Args>
    void concurrent_queue<T>::emplace_back(Args &&...args)
    {
        std::unique_lock<std::mutex> lockq{this->_queue_mutex};
        this->_queue.push(std::forward<Args>(args)...);
        this->_cond_var.notify_one();
    }

}

#endif