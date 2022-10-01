#ifndef OBSERVER_H
#define OBSERVER_H

#include <vector>

namespace tapdev
{

    template <typename T, bool is_rvalue = std::is_rvalue_reference<T>::value>
    class Observer
    {
    public:
        using data_t = typename std::conditional<is_rvalue, typename std::decay<T>::type, const T &>::type;
        virtual void update(data_t &&data) = 0;
    };

    template <typename T, bool is_rvalue = std::is_rvalue_reference<T>::value>
    class Subject
    {
    public:
        using data_t = typename std::conditional<is_rvalue, typename std::decay<T>::type, const T &>::type;
        void attach(Observer<T> *_observer)
        {
            _observers.push_back(_observer);
        }

        void notify(data_t data)
        {
            for (auto &observer : _observers)
            {
                observer->update(std::move(data));
            }
        }

    private:
        std::vector<Observer<T> *> _observers{};
    };
}

#endif