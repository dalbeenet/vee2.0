#include <vee/base.h>
#include <vee/timer.h>
#include <vee/io/boost_io_service.h>
#include <boost/asio/steady_timer.hpp>
#include <atomic>

namespace /*unnamed*/ {
template <typename T>
inline bool compare_and_swap_strong(::std::atomic<T>& dst, int exp, int value)
{
    return dst.compare_exchange_strong(exp, value);
}
}

namespace vee {

class xasync_timer: public async_timer
{
    DISALLOW_COPY_AND_ASSIGN(xasync_timer);
    DISALLOW_MOVE_AND_ASSIGN(xasync_timer);
public:
    using io_service_t = ::boost::asio::io_service;
    using second_type = second_type;
    using millisecond_type = millisecond_type;
    using timer_tick = async_timer::timer_tick;
    using delegate_t = async_timer::delegate_t;
    xasync_timer();
    virtual ~xasync_timer();
    virtual bool run(const unsigned int time_period_ms, const delegate_t& callback) override;
    virtual bool run(const second_type time_period, const delegate_t& callback) override;
    virtual bool run(const millisecond_type time_period, const delegate_t& callback) override;
    virtual bool stop() override;
    virtual bool is_running() const override;
protected:
    void _set_timer();
    void _on_timer(const boost::system::error_code& error, ::boost::asio::steady_timer* pTimer);
protected:
    io_service_t  _io_service;
    io_service_t::work _io_work;
    delegate_t    _e;
    timer_tick    _tick;
    ::boost::asio::steady_timer _timer;
    ::std::atomic<int> _running_state;
    ::std::chrono::milliseconds _period;
    ::std::thread _worker_thread;
    enum class _state_id: int
    {
        IDLE = 0,
        RUN,
    };
};

std::shared_ptr<async_timer> create_timer()
{
    return std::make_shared<xasync_timer>();
}

xasync_timer::xasync_timer():
_io_service(),
_io_work(_io_service),
_timer(_io_service),
_worker_thread(boost::bind(&boost::asio::io_service::run, &_io_service))
{
    _worker_thread.detach();
}

xasync_timer::~xasync_timer()
{
    stop();
}

bool xasync_timer::run(const unsigned int time_period_ms, const delegate_t& callback)
{
    if (compare_and_swap_strong(_running_state, (int)_state_id::IDLE, (int)_state_id::RUN) == false)
        return false;
    _e = callback;
    _tick = 0;
    _running_state = true;
    _period = ::std::chrono::milliseconds::duration(time_period_ms);
    _set_timer();
    return true;
}

bool xasync_timer::run(const second_type time_period, const delegate_t& callback)
{
    if (compare_and_swap_strong(_running_state, (int)_state_id::IDLE, (int)_state_id::RUN) == false)
        return false;
    _e = callback;
    _tick = 0;
    _running_state = true;
    _period = ::std::chrono::milliseconds::duration(time_period.value * 1000);
    _set_timer();
    return true;
}

bool xasync_timer::run(const millisecond_type time_period, const delegate_t& callback)
{
    if (compare_and_swap_strong(_running_state, (int)_state_id::IDLE, (int)_state_id::RUN) == false)
        return false;
    _e = callback;
    _tick = 0;
    _running_state = true;
    _period = ::std::chrono::milliseconds::duration(time_period.value);
    _set_timer();
    return true;
}

bool xasync_timer::stop()
{
    if (compare_and_swap_strong(_running_state, (int)_state_id::RUN, (int)_state_id::IDLE) == false)
        return false;
    //_stop_alram_channel.recv();
    return true;
}

bool xasync_timer::is_running() const
{
    if (_running_state.load() == (int)_state_id::RUN)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void xasync_timer::_set_timer()
{
    _timer.expires_from_now(_period);
    _timer.async_wait(
        std::bind(&xasync_timer::_on_timer,
        this,
        std::placeholders::_1,
        &_timer)
        );
}

#pragma warning(disable:4100)
void xasync_timer::_on_timer(const boost::system::error_code& error, ::boost::asio::steady_timer* pTimer)
{
    if (!error)
    {
        if (is_running() == (int)_state_id::RUN)
        {
            ++_tick;
            _e(_tick);
            _set_timer();
        }
        else
        {
            //while (!_stop_alram_channel.try_send());
        }
    }
    else
    {
        //printf("timer> error no: %d, message: %s\n", error.value(), error.message().data());
    }
}
#pragma warning(default:4100)

} // !namespace vee