#include "tcpsocket.hpp"
#include "select.hpp"

int main(int argc, char *argv[])
{
    TcpSocket lst_sock;

    CHECK_RET(lst_sock.Socket());//创建套接字
    CHECK_RET(lst_sock.Bind("0.0.0.0", 9000));//绑定地址信息
    CHECK_RET(lst_sock.Listen());//监听

    Select s; //实例化select
    s.Add(lst_sock); //添加监控
    while(1) {
        std::vector<TcpSocket> list;
        bool ret = s.Wait(&list); //开始监控
        if (ret == false) {
            continue;
        }
        //有描述就绪了，Select返回就绪的套接字列表
        for (auto sock : list) {
            //判断就绪的套接字是哪一种套接字
            //若就绪的套接字是监听套接字,则获取新连接
            if (sock.GetFd() == lst_sock.GetFd()) {
                TcpSocket cli_sock;
                bool ret = lst_sock.Accept(&cli_sock); //获取新连接
                if (ret == false) {
                    continue;
                }
                s.Add(cli_sock);
            }else {//反之，若就绪的套接字是通信套接字则进行正常通信
                std::string buf;
                ret = sock.Recv(buf); //接收数据
                if (ret == false) {
                    sock.Close(); //关闭套接字
                    //将不再监控的描述符从集合中移除
                    s.Del(sock);
                    continue;
                }
                std::cout << "client say: " << buf << std::endl;
                buf.clear(); //清空缓冲区
                printf("server say:");
                fflush(stdout);
                std::cin >> buf;
                sock.Send(buf); //发送数据
                if (ret == false) {
                    sock.Close(); //关闭套接字
                    s.Del(sock);
                }
            }
        }
    }
    lst_sock.Close();
    return 0;
}
