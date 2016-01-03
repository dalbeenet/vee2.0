#include <vee/network/net.h>
#include <array>
#define _CRT_SECURE_NO_WARNINGS
using namespace vee;
using namespace std;

#pragma warning(disable:4996)
int main()
{
    auto stream = net::tcp::create_stream();
    try
    {
        stream->connect("127.0.0.1", 5000);
        while (true)
        {
            array<unsigned char, 512> message;
            message.fill(0);
            printf("message> ");
            gets((char*)message.data());
            stream->write_some(message.data(), message.size());
        }
    }
    catch (exceptions::stream_open_failed& exception)
    {
        puts(exception.what());
    }
    catch (exceptions::stream_io_failed& exception)
    {
        puts(exception.what());
    }
    catch (std::exception& exception)
    {
        puts(exception.what());
    }
    return 0;
}