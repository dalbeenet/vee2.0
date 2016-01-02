#ifndef _VEE_STREAM_IO_BASE_H_
#define _VEE_STREAM_IO_BASE_H_

#include <vee/io/io_base.h>
#include <vee/delegate.h>

namespace vee {

namespace io {

class sync_stream abstract
{
public:
    virtual ~sync_stream() = default;
    virtual size_t write_some(const unsigned char* data, const size_t size) throw(...) = 0;
    virtual size_t read_some(unsigned char* const buffer, const size_t buf_capicity) throw(...) = 0;
};

#define VEE_ASYNC_READ_CALLBACK_ARGS ::vee::io::io_result& /*io result*/, unsigned char* const /*recieve buffer address*/, size_t /*recieve buffer size*/
#define VEE_ASYNC_READ_CALLBACK_SIGNATURE void(VEE_ASYNC_READ_CALLBACK_ARGS)
#define VEE_ASYNC_WRITE_CALLBACK_SIGNATURE void(::vee::io::io_result& /*io result*/)

class async_stream abstract
{
public:
    using async_read_delegate = delegate<VEE_ASYNC_READ_CALLBACK_SIGNATURE>;
    using async_read_callback = std::function<VEE_ASYNC_READ_CALLBACK_SIGNATURE>;
    using async_write_delegate = delegate<VEE_ASYNC_WRITE_CALLBACK_SIGNATURE>;
    using async_write_callback = std::function<VEE_ASYNC_WRITE_CALLBACK_SIGNATURE>;

    virtual ~async_stream() = default;
    virtual void async_read_some(unsigned char* const buffer, const size_t buf_capacity, const async_read_delegate& e) throw(...);
    virtual void async_read_some(unsigned char* const buffer, const size_t buf_capacity, async_read_delegate&& e) throw(...);
    virtual void async_read_some(unsigned char* const buffer, const size_t buf_capacity, async_read_delegate::shared_ptr& sptr_e) throw(...);
    virtual void async_read_some(unsigned char* const buffer, const size_t buf_capacity, async_read_delegate::shared_ptr&& sptr_e) throw(...);
    virtual void async_write_some(const unsigned char* data, const size_t len, const async_write_delegate& e) throw(...);
    virtual void async_write_some(const unsigned char* data, const size_t len, async_write_delegate&& e) throw(...);
    virtual void async_write_some(const unsigned char* data, const size_t len, async_write_delegate::shared_ptr& sptr_e) throw(...);
    virtual void async_write_some(const unsigned char* data, const size_t len, async_write_delegate::shared_ptr&& sptr_e) throw(...);

    virtual void async_read_some(unsigned char* const buffer, const size_t buf_capacity, async_read_callback e) throw(...) = 0;
    virtual void async_write_some(const unsigned char* data, const size_t len, async_write_callback e) throw(...) = 0;
};

class stream abstract: public sync_stream, public async_stream
{
public:
    virtual ~stream() = default;
};

} // !namespace io

} // !namespace vee

#endif // !_VEE_STREAM_IO_BASE_H_