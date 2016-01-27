#include <vee/io/boost_io_service.h>
#pragma warning(disable:4512)
#include <boost/utility/in_place_factory.hpp>
#pragma warning(default:4512)

using namespace vee;

io_service_sigleton::io_service_sigleton():
_io_service()
{
    _worker = boost::in_place(boost::ref(_io_service));
    worker_threads.create_thread(boost::bind(&boost::asio::io_service::run, &_io_service));
}

io_service_sigleton::~io_service_sigleton()
{
    //_io_service.stop();
    //_sigch.recv();
    _worker = boost::none;
    _io_service.stop();
    worker_threads.join_all();
}