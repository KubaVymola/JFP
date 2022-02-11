#ifndef __SOCKET_OUTPUT_H__
#define __SOCKET_OUTPUT_H__


class SocketOutput {
public:
    int create_socket(const int port);
    void send_headers();
    void send_data();
private:
    int socket_fd;
};

#endif