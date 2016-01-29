#ifndef _VEE_NETWORK_EXCEPTION_H_
#define _VEE_NETWORK_EXCEPTION_H_

#include <vee/io/exception.h>

namespace vee {

namespace exceptions {

class protocol_mismatch: public ::std::exception, public vee::exception
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

namespace rfc6455 {

class request_uri_not_found: public protocol_mismatch
{
public:
    virtual const char* to_string()
    {
        return this->what();
    }
    const char* what() __noexcept
    {
        return "Request-URI not found in the HTTP Upgrade request";
    }
};

class header_field_not_found: public protocol_mismatch
{
public:
    static const int description_capacity = 1024;
    char _desc[description_capacity];
    explicit header_field_not_found(const char* header_field_name, ...);
    virtual const char* to_string()
    {
        return this->what();
    }
    inline const char* what()
    {
        return _desc;
    }
};

} // !namespace rfc6455

class rfc6455_handshake_failure: public ::std::exception, public vee::exception
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

class rfc6455_heartbeat: public ::std::exception, public vee::exception
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

class rfc6455_connection_close: public ::std::exception, public vee::exception
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
