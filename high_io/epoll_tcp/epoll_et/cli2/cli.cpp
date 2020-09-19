#include "TcpSvr.hpp"

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        std::cout << "./cli [ip] [port]" << std::endl;
        return 0;
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSvr ts;
    if(!ts.CreateSocket())
    {
        return 0;
    }

    if(!ts.Connect(ip, port))
    {
        return 0;
    }

    while(1)
    {
        std::string msg;
        msg.assign("I am a client two~");
        ts.Send(msg);

        ts.Recv(&msg);
        printf("Server say:%s\n", msg.c_str());
        sleep(1);
    }

    ts.Close();
    return 0;
}
