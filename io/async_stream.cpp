#include <vee/io/stream.h>

namespace vee {

namespace io {

void async_stream::async_read_some(unsigned char* const buffer, const size_t buf_capacity, const async_read_delegate& e) throw(...)
{
    return this->async_read_some(buffer, buf_capacity, ::std::make_shared<async_read_delegate>(e));
}

void async_stream::async_read_some(unsigned char* const buffer, const size_t buf_capacity, async_read_delegate&& e) throw(...)
{
    return this->async_read_some(buffer, buf_capacity, ::std::make_shared<async_read_delegate>(std::move(e)));
}

void async_stream::async_read_some(unsigned char* const buffer, const size_t buf_capacity, async_read_delegate::shared_ptr& e_ptr) throw(...)
{
    auto caller = [e_ptr](io_result& result, unsigned char* const buffer, size_t buf_capacity) -> void
    {
        e_ptr->operator()(result, buffer, buf_capacity);
    };
    return this->async_read_some(buffer, buf_capacity, caller);
}

void async_stream::async_read_some(unsigned char* const buffer, const size_t buf_capacity, async_read_delegate::shared_ptr&& e_ptr) throw(...)
{
    auto caller = [e_ptr](io_result& result, unsigned char* const buffer, size_t buf_capacity) -> void
    {
        e_ptr->operator()(result, buffer, buf_capacity);
    };
    return this->async_read_some(buffer, buf_capacity, caller);
}

void async_stream::async_write_some(const unsigned char* data, const size_t len, const async_write_delegate& e) throw(...)
{
    return this->async_write_some(data, len, ::std::make_shared<async_write_delegate>(e));
}

void async_stream::async_write_some(const unsigned char* data, const size_t len, async_write_delegate&& e) throw(...)
{
    return this->async_write_some(data, len, ::std::make_shared<async_write_delegate>(::std::move(e)));
}

void async_stream::async_write_some(const unsigned char* data, const size_t len, async_write_delegate::shared_ptr& e_ptr) throw(...)
{
    auto caller = [e_ptr](io_result& result)
    {
        e_ptr->operator()(result);
    };
    return this->async_write_some(data, len, caller);
}

void async_stream::async_write_some(const unsigned char* data, const size_t len, async_write_delegate::shared_ptr&& e_ptr) throw(...)
{
    auto caller = [e_ptr](io_result& result)
    {
        e_ptr->operator()(result);
    };
    return this->async_write_some(data, len, caller);
}

} // !namespace io

} // !namespace vee