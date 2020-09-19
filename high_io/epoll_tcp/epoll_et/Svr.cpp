#include "EpollEtSvr.hpp"

#define CHECK(p) if(p != true) {return 0;}

int main()
{
    TcpSvr listen_ts;
    CHECK(listen_ts.CreateSocket());
    CHECK(listen_ts.Bind("0.0.0.0", 19998));
    CHECK(listen_ts.Listen());

    EpollSvr es;
    es.AddFd(listen_ts.GetFd(), true);

    while(1)
    {
        std::vector<TcpSvr> vec;
        if(!es.EpollWait(&vec))
        {
            continue;
        }

        for(size_t i = 0; i < vec.size(); i++)
        {
            if(listen_ts.GetFd() == vec[i].GetFd())
            {
                struct sockaddr_in peeraddr;
                TcpSvr peerts;
                listen_ts.Accept(&peeraddr, &peerts);
                printf("Have a new connection:[ip]:%s,[port]:%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

                //将新创建出来的套接字设置为非阻塞属性
                peerts.SetNonBlock();

                //将新创建出来的套接字添加到epoll的事件集合中
                es.AddFd(peerts.GetFd(), true);
            }
            else
            {
                std::string buf;
                int ret = vec[i].RecvNonBlock(&buf);
                if(!ret)
                {
                    es.DeleteFd(vec[i].GetFd());
                    vec[i].Close();
                    continue;
                }

                printf("recv data is %s\n", buf.c_str());
                buf.clear();
                buf.assign("I am server");
                vec[i].SendNonBlock(buf);
            }
        }
    }
    listen_ts.Close();
    return 0;
}
