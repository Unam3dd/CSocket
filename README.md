# CSocket
Socket Headers in C

for linux execute Makefile with make commands !

```c
#include "headers/socket.h"

// Compile : gcc .\src\socket.c .\main.c -o main.exe -lws2_32 -s -w -O1 -O2

int main(){
    SOCKET s = Socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    
    if (Connect(s,"127.0.0.1",555) != SOCKET_ERROR)
    {
        Send(s,"Hello World",0);
        char buffer[1024];
        int i = RecvData(s,buffer,1024,0);
        if (i != SOCKET_ERROR)
        {
            printf("%d : %s\n",i,buffer);
        }
    } else {
        printf("[!] Error : Connecting to Remote Host !");
    }

    return 0;
}
```

```c
#include "headers/socket.h"

// Compile : gcc .\src\socket.c .\server.c -o server.exe -lws2_32 -s -w -O1 -O2

int main()
{
    SOCKET fd = Socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    
    if (Bind(fd,555) == SOCKET_ERROR)
    {
        printf("[-] Error bind Address\n");
    }

    if (Listen(fd,5) == SOCKET_ERROR){
        printf("[-] Error Listen Address\n");
    }

    SOCKET nfd = Accept(fd);
    char b[] = "Hello World";
    Send(nfd,b,0);
    closesocket(nfd);

    return (0);
}
```
