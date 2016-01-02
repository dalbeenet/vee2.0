#include <vee/io/boost_io_service.h>
using namespace vee;

io_service_sigleton::io_service_sigleton():
_worker(this->_io_service),
_thread([this]()
{
    _io_service.run();
    while (!_sigch.try_send());
})
{
    // empty
}

io_service_sigleton::~io_service_sigleton()
{
    _io_service.stop();
    _thread.detach();
    _sigch.recv();
}