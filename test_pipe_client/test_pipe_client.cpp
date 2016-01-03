#include <vee/interprocess/pipe.h>
#include <array>
using namespace vee;
using namespace std;

#pragma warning(disable:4996)
int main()
{
    auto stream = interprocess::windows::create_named_pipe();
    try
    {
        stream->connect("\\\\.\\pipe\\test_vee_pipe", 
                        interprocess::named_pipe::create_option::open_existing,
                        interprocess::named_pipe::data_transfer_mode::iomode_message,
                        5000);
        while (true)
        {
            array<unsigned char, 512> message;
            message.fill(0);
            printf("message> ");
            gets((char*)message.data());
            stream->write_some(message.data(), strlen((char*)message.data()));
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