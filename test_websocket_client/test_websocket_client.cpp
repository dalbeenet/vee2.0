#include <vee/network/net.h>
#include <array>
using namespace vee;
using namespace std;

int main()
{
    auto stream = net::rfc6455::create_stream();
    try
    {
        stream->connect("127.0.0.1", 5000);
        while (true)
        {
            array<unsigned char, 512> message;
            message.fill(0);
            printf("message> ");
            gets((char*)message.data());
            stream->write_some(message.data(), strlen((char*)message.data()));
        }
    }
    catch (socket_connection_exception& exception)
    {
        puts(exception.what());
    }
    catch (socket_output_exception& exception)
    {
        puts(exception.what());
    }
    catch (std::exception& exception)
    {
        puts(exception.what());
    }
    return 0;
}