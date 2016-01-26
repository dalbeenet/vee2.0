#include <vee/network/tcp_stream.h>

namespace /* unnamed */ {
inline ::boost::asio::ip::address string_to_ipaddr(const char* str)
{
    return ::boost::asio::ip::address::from_string(str);
}
} // unnamed namespace


namespace vee {

namespace net {

namespace tcp {

tcp_stream::tcp_stream():
_host_io_service(io_service_sigleton::get().io_service()),
_socket(_host_io_service)
{

}

tcp_stream::tcp_stream(::boost::asio::ip::tcp::socket&& socket):
_host_io_service(socket.get_io_service()),
_socket(std::move(socket))
{

}

tcp_stream::tcp_stream(::boost::asio::io_service& io_service):
_host_io_service(io_service),
_socket(_host_io_service)
{

}

tcp_stream::tcp_stream(tcp_stream&& other):
_host_io_service(other._host_io_service),
_socket(static_cast<socket_t&&>(other._socket))
{

}

tcp_stream::~tcp_stream()
{
    //puts(__FUNCSIG__);
}

tcp_stream& tcp_stream::operator=(tcp_stream&& rhs)
{
    //_host_io_service = rhs._host_io_service;
    //_socket = static_cast<socket_t&&>(rhs._socket);
    ::std::swap(_socket, rhs._socket);
    return *this;
}

void tcp_stream::connect(const char* ip_addr, port_t port) throw(...)
{
    ::boost::system::error_code error;
    endpoint ep(string_to_ipaddr(ip_addr), port);
    _socket.connect(ep, error);
    if (error)
    {
        throw exceptions::stream_open_failed();
    }
}

void tcp_stream::async_connect(const char* ip_addr, port_t port, async_connect_callback e)
{
    auto handle_connect = [e](const boost::system::error_code& error) -> void
    {
        op_result result;
        if (error)
        {
            result.is_success = false;
            result.log = error.message();
        }
        else
        {
            result.is_success = true;
            result.log = error.message();
        }
        e(result);
    };
    endpoint ep(string_to_ipaddr(ip_addr), port);
    _socket.async_connect(ep, handle_connect);
}

size_t tcp_stream::write_some(const unsigned char* data, const size_t len) throw(...)
{
    ::boost::system::error_code error;
    size_t write_len = (size_t)_socket.write_some(::boost::asio::buffer(data, (size_t)len), error);
    if ((boost::asio::error::eof == error) || (boost::asio::error::connection_reset == error))
    {
        throw exceptions::stream_reset();
    }
    else if (error)
    {
        throw exceptions::stream_write_failed();
    }
    return write_len;
}

void tcp_stream::async_write_some(const unsigned char* data, const size_t len, async_write_callback e) throw(...)
{
    auto handle_write = [e](const boost::system::error_code& error, size_t bytes_transferred) -> void
    {
        io::io_result result;
        if ((boost::asio::error::eof == error) ||
            (boost::asio::error::connection_reset == error))
        {
            result.is_success = false;
            result.eof = true;
            result.bytes_transferred = bytes_transferred;
        }
        else if (error)
        {
            result.is_success = false;
            //result.log = error.message();
            result.bytes_transferred = bytes_transferred;
        }
        else
        {
            result.is_success = true;
            //result.log = error.message();
            result.bytes_transferred = bytes_transferred;
        }
        e(result);
    };
    /*if (_socket.is_open() == false)
    {
    throw socket_corrupted_exception();
    }*/
    _socket.async_write_some(::boost::asio::buffer(data, static_cast<size_t>(len)), handle_write);
}

size_t tcp_stream::read_some(unsigned char* const buffer, const size_t buf_capacity) throw(...)
{
    ::boost::system::error_code error;
    memset(buffer, 0, (size_t)buf_capacity);
    size_t read_len = (size_t)_socket.read_some(::boost::asio::buffer(buffer, (size_t)buf_capacity), error);
    if ((boost::asio::error::eof == error) || (boost::asio::error::connection_reset == error))
    {
        throw exceptions::stream_reset();
    }
    else if (error)
    {
        throw exceptions::stream_read_failed();
    }
    return read_len;
}

void tcp_stream::async_read_some(unsigned char* const buffer, const size_t buf_capacity, async_read_callback e) throw(...)
{
    auto handle_read = [buffer, buf_capacity, e](const boost::system::error_code& error, size_t bytes_transferred) -> void
    {
        io::io_result result;
        if ((boost::asio::error::eof == error) ||
            (boost::asio::error::connection_reset == error))
        {
            result.is_success = false;
            result.eof = true;
            result.bytes_transferred = bytes_transferred;
        }
        else if (error)
        {
            result.is_success = false;
            //result.log = error.message();
            result.bytes_transferred = bytes_transferred;
        }
        else
        {
            result.is_success = true;
            //result.log = error.message();
            result.bytes_transferred = bytes_transferred;
        }
        e(result, buffer, buf_capacity);
    };
    /*if (_socket.is_open() == false)
    {
        throw stream_corrupted_exception();
    }*/
    _socket.async_read_some(::boost::asio::buffer(buffer, static_cast<size_t>(buf_capacity)), handle_read);
}

void tcp_stream::disconnect()
{
    _socket.close();
}

tcp_stream::socketfd_t tcp_stream::socket_id() __noexcept
{
    SOCKET ret = _socket.native();
    return ret;
}

net_stream::shared_ptr create_stream()
{
    //TODO: IO_SERVICE SELECTION
    net_stream::shared_ptr stream = ::std::make_shared<tcp_stream>();
    return stream;
}

} // !namespace rfc6455

} // !namespace net

} // !namespace vee