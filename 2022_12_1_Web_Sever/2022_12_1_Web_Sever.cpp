#define _CRT_SECURE_NO_WARNINGS
/*
*   @author chenjingyu
*   @date 2022/12/1
*/
#include <iostream>
#include <WinSock2.h>
#include <stdio.h>
#include <WS2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")
using namespace std;
void sendhtml(SOCKET s, char* filename) {
    char protocol[] = "HTTP/1.0 200 OK\r\n";
    char serName[] = "server:simple web server\r\n";
    char cntLen[] = "Conten-length:2048\r\n";
    char cntType[100];
    sprintf(cntType, "Content-type:%s\r\n\r\n", "text/html");
    cout << cntType << endl;

    send(s, protocol, strlen(protocol), 0);
    send(s, serName, strlen(serName), 0);
    send(s, cntLen, strlen(cntLen), 0);
    send(s, cntType, strlen(cntType), 0);

    FILE* pfile = fopen(filename, "r");
    if (pfile == NULL) {
        cout << "can not open the file!" << endl;
        return;
    }
    char temp[1024] = "";
    do {
        fgets(temp, 1024, pfile);
        send(s, temp, strlen(temp), 0);
    } while (!feof(pfile));

}
int main()
{
    WSADATA wsadata;//初始化网络环境
    cout<< "welcome to my web server!" << endl;
    int is_OK = WSAStartup(MAKEWORD(2, 2),&wsadata);//使用2.2版本的socket
    if (is_OK != 0)//创建失败
    {
        cout << "WSAstartup faild: " << is_OK << endl;
        return 1;
    }
    //创建服务器流式套接字 ipv4 tcp协议
    SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in Seraddr;
    Seraddr.sin_family = AF_INET;//协议
    Seraddr.sin_port = htons(80);//HTTP端口
    Seraddr.sin_addr.s_addr = INADDR_ANY;//本机 ip地址

    //绑定套接字
    is_OK = bind(server, (sockaddr*)&Seraddr, sizeof(Seraddr));
    if (is_OK != 0)//创建失败
    {
        cout << "SOCKET_ ERROR(bind): " << is_OK << endl;
        return 1;
    }

    //监听
    is_OK = listen(server, 10);
    if (is_OK != 0)//创建失败
    {
        cout << "SOCKET_ ERROR(listen): " << is_OK << endl;
        return 1;
    }

    //客户端初始化
    sockaddr_in Cliaddr;
    int num = sizeof(Cliaddr);
    SOCKET client = accept(server, (sockaddr*)&Cliaddr, &num);

    char revdata[1024] = "";
    recv(client, revdata, 1024, 0);
    cout << "接收到的数据：" << revdata << endl;
    cout << "共接收数据大小" << strlen(revdata) << endl;

    char* filename = (char*)"index.html";
    sendhtml(client, filename);

    closesocket(server);
    WSACleanup();
    return 0;
}
