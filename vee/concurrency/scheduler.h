#ifndef _VEE_CONCURRENCY_SCHEDULER_H_
#define _VEE_CONCURRENCY_SCHEDULER_H_

#include <vee/concurrency/actor.h>

namespace vee {

#pragma warning(disable:4127)
template <class FTy>
class scheduler
{
public:
    scheduler(unsigned int init_actors, unsigned int mailbox_size):
        actor_group(init_actors, mailbox_size)
    {

    }
    ~scheduler()
    {

    }
    void force_clear_actors()
    {
        actor_group.clear_actors();
    }
    template <class Delegate, typename ...FwdArgs>
    inline bool request(Delegate&& _delegate, FwdArgs&& ... args)
    {
        /*static_assert(std::is_same<decltype(std::make_tuple(args ...)), argstuple_t>::value, "arguments can not convert to argstuple!");
        argstuple_t argstuple = std::make_tuple(args ...);*/
        return this->request_with_argstuple(std::forward<Delegate>(_delegate), std::make_tuple(args ...));
    }
    template <class Delegate, class ArgsTuple>
    bool request_with_argstuple(Delegate&& _delegate, ArgsTuple&& _tuple)
    {
        actor<FTy>* current_actor = nullptr;
        while (true)
        {
            int i = _get_current_actor_index();
            if (i < 0)
                return false;
            current_actor = actor_group.at(i);
            if (current_actor == nullptr)
            {
                continue;
            }
            if (current_actor->request_with_argstuple(
                std::forward<Delegate>(_delegate),
                std::forward<ArgsTuple>(_tuple)) >= 0)
            {
                break;
            }
        }
        return true;
    }
private:
    int _get_current_actor_index()
    {
        static std::atomic<int> counter = 0;
        int i = counter++;
        int capacity = actor_group.capacity();
        if (capacity == 0)
            return -1;
        i %= capacity;
        return i;
    }
public:
    actor_group<FTy> actor_group;
};
#pragma warning(default:4127)

} // !namespace vee

#endif // !_VEE_CONCURRENCY_SCHEDULER_H_
