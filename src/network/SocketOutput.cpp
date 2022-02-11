#include "SocketOutput.hpp"

#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>


int SocketOutput::create_socket(const int port) {
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0) return 0;

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

    // TODO make address a CLI option
    // Convert address to binary form
    if (!inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)) return 0;

    // Connect to the server
    if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) return 0;

    std::cout << "Socket connected!" << std::endl;

    return socket_fd;
}

void SocketOutput::send_headers() {
    const char data[] = "Test data\n";

    int n = send(socket_fd, data, strlen(data), 0);
    if (n < 0) std::cout << "ERROR writing to socket" << std::endl;

    std::cout << "Done sending headers" << std::endl;
}

void SocketOutput::send_data() { }