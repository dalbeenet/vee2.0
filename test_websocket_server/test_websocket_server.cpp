#include <vee/network/net.h>
#include <thread>
using namespace vee;
using namespace std;

void session_handler(net::net_stream::shared_ptr session)
{
    try
    {
        while (true)
        {
            unsigned char buffer[1024];
            size_t bytes_transferred = session->read_some(buffer, 1024);
            printf("%d bytes transferred, cont: %s\n", bytes_transferred, buffer);
        }
    }
    catch (exceptions::stream_io_failed)
    {
        printf("client disconnected.\n");
    }
}

void async_session_handler(net::net_stream::shared_ptr session,::vee::io::io_result& io_result, unsigned char* const buffer, size_t bufsize)
{
    if (!io_result.is_success || !io_result.eof)
    {
        printf("client disconnected.\n");
        return;
    }
    printf("%d bytes transferred, cont: %s\n", io_result.bytes_transferred, buffer);
    session->async_read_some(buffer, bufsize, std::bind(async_session_handler, session, placeholders::_1, placeholders::_2, placeholders::_3));
}

unsigned char buffer[1024];

void server_thread()
{
    auto server = net::rfc6455::create_server(5000);
    unsigned int session_id = 0;
    while (true)
    {
        auto client = server->accept();
        printf("client %d connected!\n", ++session_id);
        client->async_read_some(buffer, 1024, std::bind(async_session_handler, client, placeholders::_1, placeholders::_2, placeholders::_3));
        //std::thread handler(session_handler, client);
        //handler.detach();
    }
}

int main()
{
    server_thread();
    return 0;
}