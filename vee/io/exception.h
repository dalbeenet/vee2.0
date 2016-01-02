#ifndef _VEE_IO_EXCEPTION_H_
#define _VEE_IO_EXCEPTION_H_

#include <vee/exception/exception_base.h>

namespace vee {

class stream_corrupted_exception: public ::vee::exception
{
public:
    const char* what() __noexcept
    {
        return "invalid stream";
    }
};

class stream_io_exception: public ::std::exception, public ::vee::exception
{ 
public:
    const char* what() __noexcept
    {
        return "stream io failed";
    }
};

class stream_output_exception: public stream_io_exception
{
public:
    const char* what() __noexcept
    {
        return "stream output failed";
    }
};

class stream_input_exception: public stream_io_exception
{
public:
    const char* what() __noexcept
    {
        return "stream input failed";
    }
};

} // !namespace vee

#endif // !_VEE_IO_EXCEPTION_H_
