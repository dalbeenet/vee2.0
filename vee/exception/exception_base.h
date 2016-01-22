#ifndef _VEE_EXCEPTION_EXCEPTION_BASE_H_
#define _VEE_EXCEPTION_EXCEPTION_BASE_H_

#include <vee/base.h>
#include <exception>

namespace vee {

class exception abstract
{
public:
    virtual const char* to_string() = 0;
};

} // !namespace vee

#endif // !_VEE_EXCEPTION_EXCEPTION_BASE_H_
