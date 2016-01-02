#ifndef _VEE_CONCURRENCY_MUTEX_H_
#define _VEE_CONCURRENCY_MUTEX_H_

#include <atomic>
#include <mutex>
#include <vee/base.h>

namespace vee {

struct empty_mutex
{
    __forceinline void lock() {};
    __forceinline bool try_lock()
    {
        return true;
    }
    __forceinline void unlock() {};
    __forceinline std::mutex::native_handle_type native_handle()
    {
        return NULL;
    }
};

class spin_lock
{
    DISALLOW_COPY_AND_ASSIGN(spin_lock);
public:
    spin_lock();
    inline void lock(std::memory_order order = std::memory_order_acquire)
    {
        while (_lock.test_and_set(order));
    }
    inline bool try_lock(std::memory_order order = std::memory_order_acquire)
    {
        return _lock.test_and_set(order);
    }
    inline void unlock(std::memory_order order = std::memory_order_release)
    {
        _lock.clear(order);
    }
protected:
    std::atomic_flag _lock;
};

} // !namespace vee

#endif // !_VEE_CONCURRENCY_MUTEX_H_