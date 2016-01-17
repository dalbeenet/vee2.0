#include <vee/network/net.h>
#include <array>
using namespace vee;
using namespace std;

using session_t = net::net_stream::shared_ptr;

void handler(uint32_t session_id, session_t session, io::io_result& io_result, unsigned char* const buf_addr, size_t buf_size)
{
    if (!io_result.is_success || io_result.eof)
    {
        printf("client %d disconnected.\n", session_id);
        delete[] buf_addr;
        return;
    }
    printf("client %d says: %s\n", session_id, buf_addr);
    session->async_read_some(buf_addr, buf_size, ::std::bind(handler, session_id, session, placeholders::_1, placeholders::_2, placeholders::_3));
}

void server_main()
{
    auto server = net::tcp::create_server(5000);
    unsigned int session_id = 0;
    while (true)
    {
        auto session = server->accept();
        printf("client %d connected!\n", ++session_id);
        unsigned char* buffer = new unsigned char[512];
        session->async_read_some(buffer, 512, ::std::bind(handler, session_id, session, placeholders::_1, placeholders::_2, placeholders::_3));
    }
}

int main()
{
    server_main();
    return 0;
}