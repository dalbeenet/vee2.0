#ifndef _VEE_NETWORK_EXCEPTION_H_
#define _VEE_NETWORK_EXCEPTION_H_

#include <vee/io/exception.h>

namespace vee {

namespace exceptions {

class protocol_mismatch_exception: public std::exception, public vee::exception
{
public:
    virtual const char* to_string()
    {
        return this->what();
    }
    const char* what() __noexcept
    {
        return "protocol mismatch";
    }
};

class rfc6455_handshake_failure: public std::exception, public vee::exception
{
public:
    virtual const char* to_string()
    {
        return this->what();
    }
    const char* what() __noexcept
    {
        return "RFC6455 handshake failure";
    }
};

class rfc6455_heartbeat: public std::exception, public vee::exception
{
public:
    virtual const char* to_string()
    {
        return this->what();
    }
    const char* what() __noexcept
    {
        return "RFC6455 heartbeat";
    }
};

class rfc6455_connection_close: public std::exception, public vee::exception
{
public:
    virtual const char* to_string()
    {
        return this->what();
    }
    const char* what() __noexcept
    {
        return "RFC6455 connection close signal";
    }
};

} // !namespace exceptions

} // !namespace vee

#endif // !_VEE_NETWORK_EXCEPTION_H_
