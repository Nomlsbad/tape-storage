#ifndef TAPE_STORAGE_FUTURE_H
#define TAPE_STORAGE_FUTURE_H

#include <future>

template<typename T>
std::future_status getStatus(std::future<T>& future)
{
    using namespace std::chrono_literals;
    return future.wait_for(0s);
}

template<typename T>
bool isReady(std::future<T>& future)
{
    return getStatus(future) == std::future_status::ready;
}

template<typename T>
bool isReadyOrDeffered(std::future<T>& future)
{
    auto status = getStatus(future);
    return status == std::future_status::ready || status == std::future_status::deferred;
}

#endif //TAPE_STORAGE_FUTURE_H
