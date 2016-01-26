#include <vee/network/tcp_stream.h>

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
    _acceptor.close();
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
        throw exceptions::stream_accept_failed();
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

net_server::shared_ptr create_server(port_t port)
{
    ::std::shared_ptr<net_server> server = ::std::make_shared<tcp_server>(port);
    return server;
}

} // !namespace tcp

} // !namespace net

} // !namespace vee