#include <vee/network/tcp_stream.h>

namespace vee {

namespace net {

namespace tcp {

net_server::shared_ptr create_server(port_t port)
{
    ::std::shared_ptr<net_server> server = ::std::make_shared<tcp_server>(port);
    return server;
}

} // !namespace tcp

} // !namespace net

} // !namespace vee