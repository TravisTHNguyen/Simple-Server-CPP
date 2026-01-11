#include "server.h"
#include <stdio.h>
struct Server server_constructor (int domain, int protocol, int service, u_long interface, int port, int backlog, void (*launch)(void)) {
    struct Server server;
    server.domain = domain;
    server.protocol = protocol;
    server.service = service;
    server.interface = interface;
    server.port = port;
    server.backlog = backlog;
    server.launch = launch;

    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = htonl(interface);

    server.socket = socket(domain, service, protocol);
    return server;
}
