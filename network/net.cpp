#include <vee/network/net.h>
#include <vee/test/test_object.h>

namespace vee {

namespace net {

void net_stream::async_connect(const char* ipv4, port_t port, async_connect_delegate& e)
{
    return async_connect(ipv4, port, std::make_shared<async_connect_delegate>(e));
}

void net_stream::async_connect(const char* ipv4, port_t port, async_connect_delegate&& e)
{
    return async_connect(ipv4, port, std::make_shared<async_connect_delegate>(std::move(e)));
}

void net_stream::async_connect(const char* ipv4, port_t port, async_connect_delegate::shared_ptr& sptr_e)
{
    auto caller = [sptr_e](op_result& result) -> void
    {
        sptr_e->operator()(result);
    };
    return async_connect(ipv4, port, caller);
}

void net_stream::async_connect(const char* ipv4, port_t port, async_connect_delegate::shared_ptr&& sptr_e)
{
    auto caller = [sptr_e](op_result& result) -> void
    {
        sptr_e->operator()(result);
    };
    return async_connect(ipv4, port, caller);
}

void net_server::async_accept(async_accept_delegate_t e)
{
    return async_accept(std::make_shared<async_accept_delegate_t>(e));
}

void net_server::async_accept(std::shared_ptr<async_accept_delegate_t> e)
{
    auto caller = [e](op_result& result, ::std::shared_ptr<net_stream> stream) -> void
    {
        e->operator()(result, stream);
    };
    return async_accept(caller);
}

namespace rfc6455 {

size_t websocket_stream::write_some(const unsigned char* data, size_t len) throw(...)
{
    websocket_io_result result = write_some(opcode_id::binary_frame, data, len);
    return result.header_size + result.payload_size;
}

void websocket_stream::async_write_some(const unsigned char* data, size_t len, async_write_callback e) throw(...)
{
    return async_write_some(opcode_id::binary_frame, data, len, e);
}

void websocket_stream::async_write_some(opcode_id opcode, const unsigned char* data, size_t len, async_write_delegate& e) throw(...)
{
    return async_write_some(opcode, data, len, std::make_shared<async_write_delegate>(e));
}

void websocket_stream::async_write_some(opcode_id opcode, const unsigned char* data, size_t len, async_write_delegate&& e) throw(...)
{
    return async_write_some(opcode, data, len, std::make_shared<async_write_delegate>(std::move(e)));
}

void websocket_stream::async_write_some(opcode_id opcode, const unsigned char* data, size_t len, async_write_delegate::shared_ptr& e_ptr) throw(...)
{
    auto caller = [e_ptr](io::io_result& result) -> void
    {
        e_ptr->operator()(result);
    };
    return async_write_some(opcode, data, len, caller);
}

void websocket_stream::async_write_some(opcode_id opcode, const unsigned char* data, size_t len, async_write_delegate::shared_ptr&& e_ptr) throw(...)
{
    auto caller = [e_ptr](io::io_result& result) -> void
    {
        e_ptr->operator()(result);
    };
    return async_write_some(opcode, data, len, caller);
}

size_t websocket_stream::read_some(unsigned char* const buffer, size_t buf_capacity) throw(...)
{
    return read_some_payload_only(buffer, buf_capacity).payload_size;
}

void websocket_stream::async_read_some(unsigned char* const buffer, size_t buf_capacity, async_read_callback e) throw(...)
{
    return async_read_some_payload_only(buffer, buf_capacity, e);
}

void websocket_stream::async_read_some_payload_only(unsigned char* const buffer, size_t buf_capacity, async_read_delegate& e) throw(...)
{
    return async_read_some_payload_only(buffer, buf_capacity, std::make_shared<async_read_delegate>(e));
}

void websocket_stream::async_read_some_payload_only(unsigned char* const buffer, size_t buf_capacity, async_read_delegate&& e) throw(...)
{
    return async_read_some_payload_only(buffer, buf_capacity, std::make_shared<async_read_delegate>(std::move(e)));
}

void websocket_stream::async_read_some_payload_only(unsigned char* const buffer, size_t buf_capacity, async_read_delegate::shared_ptr& e_ptr) throw(...)
{
    auto caller = [e_ptr](io::io_result& result, unsigned char* const buffer, size_t buf_capacity) -> void
    {
        e_ptr->operator()(result, buffer, buf_capacity);
    };
    return async_read_some_payload_only(buffer, buf_capacity, caller);
}

void websocket_stream::async_read_some_payload_only(unsigned char* const buffer, size_t buf_capacity, async_read_delegate::shared_ptr&& e_ptr) throw(...)
{
    auto caller = [e_ptr](io::io_result& result, unsigned char* const buffer, size_t buf_capacity) -> void
    {
        e_ptr->operator()(result, buffer, buf_capacity);
    };
    return async_read_some_payload_only(buffer, buf_capacity, caller);
}

void websocket_stream::async_read_some_all(unsigned char* const buffer, size_t buf_capacity, async_read_delegate& e) throw(...)
{
    return async_read_some_all(buffer, buf_capacity, std::make_shared<async_read_delegate>(e));
}

void websocket_stream::async_read_some_all(unsigned char* const buffer, size_t buf_capacity, async_read_delegate&& e) throw(...)
{
    return async_read_some_all(buffer, buf_capacity, std::make_shared<async_read_delegate>(std::move(e)));
}

void websocket_stream::async_read_some_all(unsigned char* const buffer, size_t buf_capacity, async_read_delegate::shared_ptr& e) throw(...)
{
    auto caller = [e](io::io_result& result, unsigned char* const buffer, size_t buf_capacity) -> void
    {
        e->operator()(result, buffer, buf_capacity);
    };
    return async_read_some_all(buffer, buf_capacity, caller);
}

void websocket_stream::async_read_some_all(unsigned char* const buffer, size_t buf_capacity, async_read_delegate::shared_ptr&& e) throw(...)
{
    auto caller = [e](io::io_result& result, unsigned char* const buffer, size_t buf_capacity) -> void
    {
        e->operator()(result, buffer, buf_capacity);
    };
    return async_read_some_all(buffer, buf_capacity, caller);
}

} // !namespace rfc6455

} // !namespace net

} // !namespace vee