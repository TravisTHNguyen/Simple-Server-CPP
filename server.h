#ifndef server_h
#define server_h
#include <sys/socket.h>
#include <netinet/in.h>

struct Server {
    int domain, protocol, service;
    u_long interface;
    int port, backlog;
    int socket;
    struct sockaddr_in address;
    void (*launch)(void);
};
struct Server server_constructor(int domain, int protocol, int service, u_long interface, int port, int backlog, void(*launch)(void));
#endif
