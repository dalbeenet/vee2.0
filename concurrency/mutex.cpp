#include <vee/concurrency/mutex.h>
using namespace vee;

spin_lock::spin_lock()
{
    _lock.clear();
}