#include "SelectSvr.hpp"

#define CHECK_RET(p) if(p != true) {return -1;}

int main()
{
    TcpSvr listen_ts;
    CHECK_RET(listen_ts.CreateSocket());
    CHECK_RET(listen_ts.Bind("0.0.0.0", 19998));
    CHECK_RET(listen_ts.Listen());

    SelectSvr ss;
    ss.AddFd(listen_ts.GetFd());

    while(1)
    {
        //1.监控
        std::vector<TcpSvr> vec;
        if(!ss.SelectWait(&vec))
        {
            continue;
        }

        for(size_t i = 0; i < vec.size(); i++)
        {
            //2.接收新客户端的连接
            if(listen_ts.GetFd() == vec[i].GetFd())
            {
                struct sockaddr_in peeraddr;
                TcpSvr peerts;
                listen_ts.Accept(&peeraddr, &peerts);
                printf("Have a new connection: [ip]:%s, [port]:%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

                //将新创建的套接字添加到select的事件集合中
                ss.AddFd(peerts.GetFd());
            }
            //3.接收旧客户端数据的到来
            else
            {
                std::string data;
                bool ret = vec[i].Recv(&data);
                if(!ret)
                {
                    //表示接收失败或者对端关闭了连接
                    //从select事件集合中删掉当前套接字
                    ss.DeleteFd(vec[i].GetFd());
                    vec[i].Close();
                }
                printf("client send data is [%s]\n", data.c_str());
            }
        }
    }
    return 0;
}
