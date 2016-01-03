#ifndef _VEE_IO_EXCEPTION_H_
#define _VEE_IO_EXCEPTION_H_

#include <vee/exception/exception_base.h>

namespace vee {

namespace exceptions {

class stream_open_failed: public std::exception, public ::vee::exception
{
public:
    const char* what() __noexcept
    {
        return "stream creation failed";
    }
};

class stream_already_opened: public stream_open_failed
{
public:
    const char* what() __noexcept
    {
        return "stream is already opened";
    }
};

class stream_corrupted: public std::exception, public ::vee::exception
{
public:
    const char* what() __noexcept
    {
        return "invalid stream";
    }
};

class stream_reset: public ::std::exception, public ::vee::exception
{
public:
    const char* what() __noexcept
    {
        return "stream reset";
    }
};

class stream_io_failed: public ::std::exception, public ::vee::exception
{
public:
    const char* what() __noexcept
    {
        return "stream io failed";
    }
};

class stream_write_failed: public stream_io_failed
{
public:
    const char* what() __noexcept
    {
        return "stream output failed";
    }
};

class stream_read_failed: public stream_io_failed
{
public:
    const char* what() __noexcept
    {
        return "stream input failed";
    }
};

class stream_accept_failed: public ::std::exception, ::vee::exception
{
public:
    const char* what() __noexcept
    {
        return "stream accept failed";
    }
};

} // !namespace exceptions

} // !namespace vee

#endif // !_VEE_IO_EXCEPTION_H_
