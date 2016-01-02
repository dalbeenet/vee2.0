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

tcp_server::tcp_server(unsigned short port, ::boost::asio::io_service& io_service):
_host_io_service(io_service),
_socket(_host_io_service),
_endpoint(boost::asio::ip::tcp::v4(), port),
_acceptor(_host_io_service, _endpoint)
{

}

tcp_server::tcp_server(tcp_server&& other):
_host_io_service(other._host_io_service),
_socket(static_cast<socket_t&&>(other._socket)),
_endpoint(static_cast<endpoint&&>(other._endpoint)),
_acceptor(static_cast<::boost::asio::ip::tcp::acceptor&&>(other._acceptor))
{

}

tcp_server::~tcp_server()
{

}

void tcp_server::close()
{
    _socket.close();
}

net_stream::shared_ptr tcp_server::accept()
{
    ::boost::asio::ip::tcp::socket client(_host_io_service);
    try
    {
        _acceptor.accept(client);
    }
    catch (::std::exception&)
    {
        throw socket_accept_exception();
    }
    net_stream::shared_ptr ret = ::std::make_shared<tcp_stream>(std::move(client));
    return ret;
}

void tcp_server::async_accept(std::function<VEE_NET_ASYNC_ACCEPT_CALLBACK_SIGNATURE> e)
{
    std::shared_ptr<::boost::asio::ip::tcp::socket> socket = std::make_shared<::boost::asio::ip::tcp::socket>(_host_io_service);
    auto handle_accept = [socket, e](const boost::system::error_code& error) -> void
    {
        op_result result;
        if (error)
        {
            result.is_success = false;
            result.log += error.message();
            net_stream::shared_ptr ptr(nullptr);
            e(result, ptr);
        }
        else
        {
            result.is_success = true;
            result.log += error.message();
            e(result, ::std::make_shared<tcp_stream>(std::move(*socket)));
        }
    };
    _acceptor.async_accept(*socket, handle_accept);
}

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
        throw socket_connection_exception();
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
    if (error)
    {
        throw socket_output_exception();
    }
    return write_len;
}

void tcp_stream::async_write_some(const unsigned char* data, const size_t len, async_write_callback e) throw(...)
{
    auto handle_write = [e](const boost::system::error_code& error, size_t bytes_transferred) -> void
    {
        io::io_result result;
        if (error)
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
    if (error)
    {
        throw socket_input_exception();
    }
    return read_len;
}

void tcp_stream::async_read_some(unsigned char* const buffer, const size_t buf_capacity, async_read_callback e) throw(...)
{
    auto handle_read = [buffer, buf_capacity, e](const boost::system::error_code& error, size_t bytes_transferred) -> void
    {
        io::io_result result;
        if (error)
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

net_stream::shared_ptr create_stream()
{
    //TODO: IO_SERVICE SELECTION
    net_stream::shared_ptr stream = ::std::make_shared<tcp_stream>();
    return stream;
}

} // !namespace rfc6455

} // !namespace net

} // !namespace vee