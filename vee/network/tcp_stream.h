#ifndef _VEE_NETWORK_TCP_STREAM_H_
#define _VEE_NETWORK_TCP_STREAM_H_

#include <vee/network/net.h>
#include <vee/io/boost_io_service.h>

namespace vee {

namespace net {

namespace tcp {

class tcp_server;
class tcp_stream;

#define VEE_NET_ASYNC_CONNECT_CALLBACK_SIGNATURE void(::vee::net::op_result&/*operation result*/)
#define VEE_NET_ASYNC_ACCEPT_CALLBACK_SIGNATURE  void(::vee::net::op_result&/*operation result*/, ::vee::net::net_stream::shared_ptr/*stream*/)

class tcp_stream: public net_stream
{
    DISALLOW_COPY_AND_ASSIGN(tcp_stream);
public:
    using unique_ptr = ::std::unique_ptr<tcp_stream>;
    using shared_ptr = ::std::shared_ptr<tcp_stream>;
    using socket_t = ::boost::asio::ip::tcp::socket;
    using endpoint = ::boost::asio::ip::tcp::endpoint;
    using io_service_t = ::boost::asio::io_service;
    tcp_stream();
    virtual ~tcp_stream();
    explicit tcp_stream(socket_t&& socket);
    explicit tcp_stream(io_service_t& io_service);
    tcp_stream(tcp_stream&& other);
    tcp_stream& operator=(tcp_stream&& rhs);
    virtual void        connect(const char* ip_addr, port_t port) throw(...) override;
    virtual void        async_connect(const char* ip_addr, port_t port, async_connect_callback e) override;
    virtual void        disconnect() override;
    virtual size_t      write_some(const unsigned char* data, const size_t len) throw(...) override;
    virtual void        async_write_some(const unsigned char* data, const size_t len, async_write_callback e) throw(...) override;
    virtual size_t      read_some(unsigned char* const buffer, const size_t buf_capacity) throw(...) override;
    virtual void        async_read_some(unsigned char* const buffer, const size_t buf_capacity, async_read_callback e) throw(...) override;
    virtual socketfd_t  socket_id() __noexcept override;
    inline io_service_t& get_io_service() const
    {
        return _host_io_service;
    }
protected:
    io_service_t& _host_io_service;
    socket_t _socket;
};

class tcp_server: public net_server
{
    DISALLOW_COPY_AND_ASSIGN(tcp_server);
public:
    using unique_ptr = ::std::unique_ptr<tcp_server>;
    using shared_ptr = ::std::shared_ptr<tcp_server>;
    using socket_t = ::boost::asio::ip::tcp::socket;
    using endpoint = ::boost::asio::ip::tcp::endpoint;
    using io_service_t = ::boost::asio::io_service;
    tcp_server(unsigned short port, io_service_t& io_service = io_service_sigleton::get().io_service());
    tcp_server(tcp_server&& other);
    virtual ~tcp_server();
    virtual void close() override;
    virtual net_stream::shared_ptr accept() throw(...) override;
    virtual void async_accept(std::function<VEE_NET_ASYNC_ACCEPT_CALLBACK_SIGNATURE> e) override;
    inline io_service_t& get_io_service() const
    {
        return _host_io_service;
    }
protected:
    io_service_t& _host_io_service;
    ::boost::asio::ip::tcp::endpoint _endpoint;
    ::boost::asio::ip::tcp::acceptor _acceptor;
    socket_t _socket;
};

} // !namespace tcp

} // !namespace net

} // !namespace vee

#endif // !_VEE_NETWORK_TCP_STREAM_H_
