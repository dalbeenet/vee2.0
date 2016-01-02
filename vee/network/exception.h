#ifndef _VEE_NETWORK_EXCEPTION_H_
#define _VEE_NETWORK_EXCEPTION_H_

#include <vee/io/exception.h>

namespace vee {

class socket_accept_exception: public ::std::exception, ::vee::exception
{
public:
    const char* what() __noexcept
    {
        return "accept failed";
    }
};

class socket_corrupted_exception: public stream_corrupted_exception
{
public:
    const char* what() __noexcept
    {
        return "invalid socket";
    }
};

class socket_connection_exception: public ::std::exception, ::vee::exception
{
public:
    const char* what() __noexcept
    {
        return "socket connection failed";
    }
};

class socket_io_exception: public stream_io_exception
{
public:
    const char* what() __noexcept
    {
        return "socket I/O failed";
    }
};

class socket_output_exception: public socket_io_exception
{
public:
    const char* what() __noexcept
    {
        return "socket write failed";
    }
};

class socket_input_exception: public socket_io_exception
{
public:
    const char* what() __noexcept
    {
        return "socket read failed";
    }
};

class protocol_mismatch_exception: public std::exception, public vee::exception
{
public:
    const char* what() __noexcept
    {
        return "protocol mismatch";
    }
};

class rfc6455_handshake_failure: public std::exception, public vee::exception
{
public:
    const char* what() __noexcept
    {
        return "RFC6455 handshake failure";
    }
};

class rfc6455_heartbeat: public std::exception, public vee::exception
{
public:
    const char* what() __noexcept
    {
        return "RFC6455 heartbeat";
    }
};

class rfc6455_connection_close: public std::exception, public vee::exception
{
public:
    const char* what() __noexcept
    {
        return "RFC6455 connection close signal";
    }
};

} // !namespace vee

#endif // !_VEE_NETWORK_EXCEPTION_H_
