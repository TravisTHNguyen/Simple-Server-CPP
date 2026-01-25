
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
int handle_client(int client_socket) {
    ssize_t n = 0;
    char buf[1024];
    const char* hello = "HTTP/1.0 200 OK\r\n\r\nHello, World!";
    for(;;) {
        memset(buf,0,sizeof(buf));
        n = read(client_socket,buf, sizeof(buf)-1);
        if(n < 0) {
            perror("read(client)");
            return -1;
        }
        if(n == 0) {
            printf("connection closed\n");
            break;
        }
        printf("REQUEST:\n%s", buf);
        (void)write(client_socket,hello,strlen(hello));
        close(client_socket);
        break;

    }
    printf("\n--\n");
    return 0;
}
/*TCP information taken from Arch Linux TCP 'man' pages */
int main(void) {
    int rc = 0;
    struct sockaddr_in bind_addr;
    int tcp_socket = 0;
    int ret = 0;
    int client_socket = 0;
    int enabled = true;
    memset(&bind_addr, 0, sizeof(bind_addr));

    tcp_socket = socket(AF_INET,  /*IPv4 */
        SOCK_STREAM, /*TCP */
        0);

    if (tcp_socket < 0) {
        perror("socket");
        return 1;
    }

    printf("socket creation succeeded\n");

    (void)setsockopt(tcp_socket, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled));

    bind_addr.sin_port = htons(6969);
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_addr.s_addr = INADDR_ANY;

    rc = bind(tcp_socket, (const struct sockaddr*) &bind_addr, sizeof(bind_addr));
    if (rc < 0) {
        perror("bind()");
        ret = 1;
        goto exit;
    }

    printf("bind successful\n");
    rc = listen(tcp_socket, SOMAXCONN);

    if (rc < 0) {
        perror("listen()");
        ret = 1;
        goto exit;
    }

    printf("listen successful\n");

    for(;;) {
        ssize_t n = 0;
        printf("waiting for connection..\n");
        client_socket = accept(tcp_socket, NULL, NULL);

        printf("got connection\n");
        rc = handle_client(client_socket);
        /*ignore errors, don't care for now */
    }
exit:
    close(tcp_socket);
    return ret;
}
