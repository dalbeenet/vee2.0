#include <vee/interprocess/pipe.h>
#include <thread>
using namespace vee;
using namespace std;

#pragma warning(disable:4996)
using session_t = interprocess::named_pipe::shared_ptr;

void session_handler(session_t session)
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

void async_session_handler(session_t session, ::vee::io::io_result& io_result, unsigned char* const buffer, size_t bufsize)
{
    if (!io_result.is_success)
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
    auto server = interprocess::windows::create_named_pipe_server();
    unsigned int session_id = 0;
    while (true)
    {
        auto client = server->accept("\\\\.\\pipe\\test_vee_pipe",
                                     interprocess::named_pipe::data_transfer_mode::iomode_message,
                                     1024,
                                     1024);
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