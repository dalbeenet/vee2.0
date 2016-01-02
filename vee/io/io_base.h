#ifndef _VEE_IO_IO_BASE_H_
#define _VEE_IO_IO_BASE_H_

#include <cinttypes>

namespace vee {

namespace io {

struct io_result
{
    bool is_success = 0;
    size_t bytes_transferred = 0;
};

} // !namespace io

}; // !namespace vee

#endif // !_VEE_IO_IO_BASE_H_
