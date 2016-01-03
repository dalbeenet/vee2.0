#ifndef _VEE_INTERPROCESS_PIPE_H_
#define _VEE_INTERPROCESS_PIPE_H_

#include <vee/delegate.h>
#include <vee/io/stream.h>
#include <vee/interprocess/exception.h>

namespace vee {

namespace interprocess {

struct pipe_time_out_constant
{
    static const int wait_default = 0x00000000;
    static const int wait_forever = 0xffffffff;
    static const int wait_nowait = 0x00000001;
};

class named_pipe abstract: public io::stream
{
public:
    enum class create_option: int
    {
        create_or_truncate, // exists -> truncates, does not exist -> creates
        create_new,         // exists -> fails, does not exist -> creates
        open_or_crate,      // exists -> opens, does not exist -> creates
        open_existing,      // exists -> opens, does not exist -> fails
        truncate_existing   // exists -> truncates, does not eixst -> fails
    };
    enum class data_transfer_mode: int
    {
        iomode_byte,      // Data is read from pipe as a stream of bytes,  This mode is the default if no read-mode flag is specified.
        iomode_message,   // Data is read from the pipe as a stream of messages. The function fails if this flag is specified for a byte-type pipe.
    };
    using unique_ptr = ::std::unique_ptr<named_pipe>;
    using shared_ptr = ::std::shared_ptr<named_pipe>;
    virtual ~named_pipe() = default;
    virtual void connect(const char* pipe_name, const create_option option, const data_transfer_mode read_mode, const size_t time_out_millisec) throw(...) = 0;
    virtual void disconnect() throw(...) = 0;
};

class named_pipe_server abstract
{
public:
    using unique_ptr = ::std::unique_ptr<named_pipe_server>;
    using shared_ptr = ::std::shared_ptr<named_pipe_server>;
    virtual ~named_pipe_server() = default;
    virtual ::std::shared_ptr<named_pipe> accept(const char* pipe_name,
                                                 const named_pipe::data_transfer_mode mode,
                                                 const size_t in_buffer_size,
                                                 const size_t out_buffer_size) throw(...) = 0;
    virtual void close() throw(...) = 0;
};

namespace windows {

named_pipe_server::shared_ptr    create_named_pipe_server() __noexcept;
named_pipe::shared_ptr           create_named_pipe() __noexcept;

} // !namespace windows

} // !namespace interprocess

} // !namespace vee

#endif // !_VEE_INTERPROCESS_PIPE_H_
